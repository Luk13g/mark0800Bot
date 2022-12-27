#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "app3.h"
#include "motor3.h"
#include "adc.h"

//bateria adc-2410 - 8v

volatile uint8_t estado;
volatile uint8_t ligar;
static uint32_t conta;


//Par�metros de distancia do sensor
//Sensor de linha
#define SETPOINT_LUZ_ESQ 2800//2830//700//2730 //a baixo desse valor  branco 700
#define SETPOINT_LUZ_DIR 2800//2830//700//2730 //700

//Sensor de dist�ncia - sensor central 4 a 30 cm /sensores laterais 10 a 80 cm
#define DETECTED_RANGE_ESQ 1200 //objeto detectado - alvo
#define DETECTED_RANGE_DIR 1200 //objeto detectado - alvo
#define DETECTED_RANGE_CENTRO 1000 //objeto detectado - alvo
#define CLOSE_RANGE_ESQ   2800 //objeto detectado sensor esquerda - ataque
#define CLOSE_RANGE_DIR   2800 //objeto detectado sensor direita - ataque
#define CLOSE_RANGE_CENTRO   2000 //2000 objeto detectado sensor esquerda - ataque

//Parametros de velocidade do motor
#define REVERSE_SPEED     12000//6000//12000//6000  // 200 rpm 12000
#define TURN_SPEED        15000//7000//15000//5000 // 15000
#define FORWARD_SPEED     17500//7000//17500//6000 // 17500
#define FORWARD_SPEED_2   19000//13000//19000//13000 //19000
#define ATTACK_SPEED      19999//18000//19999//19999 //19999

//Par�metros de dura��o dos movimentos
#define REVERSE_DURATION  150 // ms 150
#define TURN_DURATION     200 // ms 200
#define HUNT_DURATION     250 //250 ms

uint32_t hw_timer_get_tick_ms(void)
{
	return HAL_GetTick();
}

// Fila de comandos
#define MAX_COMMANDS 10
uint32_t startTime; // o tempo em milissegundos em que a fila de comandos come�ou a executarCommands
uint32_t nextCommand; // o horario em que a fila deve executar o proximo comando
unsigned int commandIndex;
unsigned int numberOfCommands;
int leftSpeed[MAX_COMMANDS];
int rightSpeed[MAX_COMMANDS];
unsigned int commandDuration[MAX_COMMANDS];
unsigned int turnDirection = 1;

// Configura��o da m�quina de estado finito
// Estados
enum State {
	SURVIVE, HUNT, TARGET, ATTACK//, EVADE
}
state;

//  Eventos
bool oponenteDetectado;
bool bordaDetectada;
bool oponenteDetectado_esq;
bool oponenteDetectado_dir;
bool detectado_oponente;
//bool inContact;
bool executandoComandos;


//  Sensores de guarda
volatile extern uint32_t adc_data_avg[ADC_NUM_CHANNELS];

/*void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if( GPIO_Pin == GPIO_PIN_7 )
	{
		ligar = 1;
	}
}*/


void updateFromSensors()//logica boleana sensores
{

	bordaDetectada = ((adc_data_avg[2] < SETPOINT_LUZ_DIR) || (adc_data_avg[3] < SETPOINT_LUZ_ESQ));

	oponenteDetectado = adc_data_avg[5] >= DETECTED_RANGE_CENTRO;//Retorna 0 ou 1 - 0 detectado - 1 n�o detectado.
	oponenteDetectado_esq = adc_data_avg[0] >= DETECTED_RANGE_ESQ;
	oponenteDetectado_dir = adc_data_avg[1] >= DETECTED_RANGE_DIR;
	detectado_oponente = ((oponenteDetectado_esq)||(oponenteDetectado_dir)||(oponenteDetectado));

}

void maquina_de_estado()
{
	// verifica a transi��o ou executa o estado atual

	switch (state)
	{
	case SURVIVE:
		if (!executandoComandos)hunt();//quando n�o executar comandos
		else if (bordaDetectada) survive();
		break;
	//case EVADE:

		//else if (!executandoComandos) hunt();
		//break;
	case ATTACK:
		if (bordaDetectada) survive();
		else if (!detectado_oponente) hunt();//se oponente n�o for encontrado ele fica ca�ando
		// Oponente ainda detectado - guarda de verifica��o
		else if (oponenteDetectado) target();
		break;
	case TARGET:
		if (bordaDetectada) survive();
		//else if (inContact) evade(); - caso usar acelerometro - fun��o de fuga
		// Lost oponent
		else if (!detectado_oponente) hunt();
		// Oponente ainda detectado - guarda de verifica��o
		// Atacar se fechar
		else if (adc_data_avg[5] >= CLOSE_RANGE_CENTRO) attack();
		// Continue to target
		break;
	default:  // HUNT
		if (bordaDetectada) survive();
		//else if (inContact) evade();
		else if (detectado_oponente)
		{
			// Oponente detectado - checando
			if (adc_data_avg[5] >= CLOSE_RANGE_CENTRO) attack();
			else target();
		}
		else if (!executandoComandos) hunt();
		break;
	}
}


 // Detecta qualquer borda, o robo vai para tras, apos isso locomove conforme a borda que pegou
 //SENSOR DIREITO -> 1 ent�o deve ir para esquerda (-1,1)
 //SENSOR ESQUERDA -> -1 ent�o deve ir para direita (1,-1)
 //SEQUENCIA -> tras ->direita(pegou sensor esquerda) ou esquerda(pegou sensor direita) ->frente
 //se pegar os dois sensores (1,-1) - tras -> direita ->frente

void survive()
{


	state = SURVIVE;


	if (adc_data_avg[2] < SETPOINT_LUZ_DIR)
	{
	turnDirection = -1;

    HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);//MOTOR ESQUERDA
    HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET);
	}
	else
	{
	turnDirection = 1;
    HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET);//MOTOR ESQUERDA
    HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);
	}
	clearCommands();

	if ((adc_data_avg[2] < SETPOINT_LUZ_DIR) && (adc_data_avg[3] < SETPOINT_LUZ_ESQ))
	{
	    HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET);//MOTOR ESQUERDA
	    HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET);
		addCommand(turnDirection * TURN_SPEED, -turnDirection * TURN_SPEED, TURN_DURATION);
	}


	addCommand(-REVERSE_SPEED, -REVERSE_SPEED, REVERSE_DURATION);
	addCommand(turnDirection * TURN_SPEED, -turnDirection * TURN_SPEED, TURN_DURATION);
	addCommand(FORWARD_SPEED, FORWARD_SPEED, REVERSE_DURATION);
	startExecutingCommands();
}

// Hunt - search for the oponent

// Modo ca�a - procura oponente - vai pra frente -> pra direita ->pra esquerda, at� encontrar adversario

void hunt()
{

	state = HUNT;
    HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);//MOTOR ESQUERDA
    HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);

	clearCommands();
	addCommand(FORWARD_SPEED, FORWARD_SPEED, HUNT_DURATION);
	//addCommand(turnDirection * TURN_SPEED, -turnDirection * TURN_SPEED, HUNT_DURATION);
	//addCommand(-turnDirection * TURN_SPEED, turnDirection * TURN_SPEED, HUNT_DURATION);
	startExecutingCommands();
}

// Target - uma vez que o oponente � detectado, posicione para um ataque

 // Detec��o de alvo - vai para frente

void target()
{

	state = TARGET;

    HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);//MOTOR ESQUERDA
    HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);

	clearCommands();
	  if(oponenteDetectado_esq && !oponenteDetectado)
	  {
		  addCommand(16000, -16000, 400);//16000,-15500
	  }
	  else if(oponenteDetectado_dir && !oponenteDetectado)
	  {
		  addCommand(-16000, 16000, 400);//-15500 ,16000
	  }
	/*if (adc_data_avg[0] > 1.1* adc_data_avg[5])
	{
		addCommand(14000, 8000, 100);//mover para esquerda
	}

	else if (adc_data_avg[1] > 1.1* adc_data_avg[5])
	{
		addCommand(8000, 14000, 100);//mover para direita

	}*/
	/*if(adc_data_avg[0] < adc_data_avg[1])
	{
		while(adc_data_avg[0] < adc_data_avg[1])
		{
			addCommand(8000, 2000, 0);//mover para esquerda
			if((adc_data_avg[0] >= (1.1*adc_data_avg[1])) || bordaDetectada)
			{
				break;
			}
		}
	}
	if(adc_data_avg[0] > adc_data_avg[1])
	{
		while(adc_data_avg[0] > adc_data_avg[1])
		{
			addCommand(2000, 10000, 0);//mover para esquerda
			if((adc_data_avg[1] >= (1.1*adc_data_avg[0])) || bordaDetectada)
			{
				break;
			}
		}
	}*/
		// || (adc_data_avg[1] >= CLOSE_RANGE_DIR)
	else
	{
	addCommand(FORWARD_SPEED_2, FORWARD_SPEED_2, 0);
	}
	startExecutingCommands();
}


 //Modo ataque - adversario est� bem proximo
 // sequencia ->pau na maquina (no talo)

void attack()
{

	state = ATTACK;

    HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);//MOTOR ESQUERDA
    HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);

	clearCommands();
	addCommand(ATTACK_SPEED, ATTACK_SPEED, 300);
	startExecutingCommands();
}

// *** Fila de comandos ***
// para de executar comandos e redefine a fila
void clearCommands()
{
	executandoComandos = false;
	numberOfCommands = 0;
	nextCommand = hw_timer_get_tick_ms();
}

//come�a a executar comandos desde o in�cio da fila
void startExecutingCommands()
{
	executandoComandos = true;
	commandIndex = 0;
	startTime = hw_timer_get_tick_ms();
	nextCommand = startTime;  // executa o primeiro comando imediatamente
}

// Adiciona comando
void addCommand(int left, int right, int duration)
{
	if (numberOfCommands < MAX_COMMANDS)
	{
		// Seta os parametros do comando (velocidade do motor esquerdo, vel do motor direito, e dura��o do comando)
		leftSpeed[numberOfCommands] = left;
		rightSpeed[numberOfCommands] = right;
		commandDuration[numberOfCommands] = duration;

		// Incrementa o n�mero de comandos
		numberOfCommands++;
	}
}

void executeCommands()
{
// verifica se o comando atual est� completo
	if (hw_timer_get_tick_ms() >= nextCommand)
	{
		// Executa o proximo comando
		if (commandIndex < numberOfCommands)
		{
			setSpeeds(leftSpeed[commandIndex], rightSpeed[commandIndex]);
			nextCommand = nextCommand + commandDuration[commandIndex];
			commandIndex++;
		}
		// comandos concluidos
		else executandoComandos = false;
	}
}

void app3_init()
{
	motor3_init();
	adc_init();
	//startTime = hw_timer_get_tick_ms();
	//nextCommand = hw_timer_get_tick_ms();
	conta = 0;
	ligar = 0;
}
void app3_loop()
{
	if(ligar)
	{
		motor_parada();
		HAL_Delay(5000);
	    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
	    HAL_Delay(500);
	    HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin, GPIO_PIN_RESET);

		while(1)
		{
			updateFromSensors();
			maquina_de_estado();
			executeCommands();
		}
	}
}

