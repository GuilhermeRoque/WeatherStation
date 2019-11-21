/* 
 * File:   TWIMaster.cpp
 * Author: marcone
 * 
 * Created on 30 de Outubro de 2019, 08:36
 */

#include "TWIMaster.h"

TWIMaster::TWIMaster(unsigned long freq) {  // speed, modo (leitura/ escrita), quantidade de escravos, e array de endereços de escravos
    TWBR = ((F_CPU/freq)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1)))));
}
TWIMaster::~TWIMaster() {
}
uint8_t TWIMaster::Start(char write_address){				/* TWI start function */
	uint8_t status;											/* Declarar variável */
	TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);					/* Ative o TWI, gere condição de início e limpe o sinalizador de interrupção */
	while (!(TWCR & (1<<TWINT)));							/* Aguarde até que o TWI termine seu trabalho atual (condição inicial) */
	status = TWSR & 0xF8;									/* Leia o registro de status TWI com mascaramento de três bits inferiores */
	if (status != 0x08){									/* Verificar a condição de início foi transmitida com sucesso ou não? */
        return 0;											/* Caso contrário, retorne 0 para indicar falha na condição de início */
	}
    TWDR = write_address;									/* Se sim, escreva SLA + W no registro de dados TWI */
	TWCR = (1<<TWEN)|(1<<TWINT);							/* Ative o TWI e limpe o sinalizador de interrupção */
	while (!(TWCR & (1<<TWINT)));							/* Aguarde até o TWI concluir seu trabalho atual (operação de gravação) */
	status = TWSR & 0xF8;									/* Leia o registro de status TWI com mascaramento de três bits inferiores */	
	if (status == 0x18){									/* Verifique se o SLA + W foi transmitido e ack recebido ou não?  */
        return 1;											/* Se sim, retorne 1 para indicar a confirmação recebida, ou seja, pronto para aceitar o byte de dados */
	}
    if (status == 0x20){									/* Verifique o SLA + W foi transmitido e o nack recebido ou não? */
        return 2;											/* Se sim, retorne 2 para indicar o nack recebido, ou seja, o dispositivo está ocupado */
	}else
        return 3;											/* Caso contrário, retorne 3 para indicar falha no SLA + W */
}

/*INÍCIO REPETIDO(Sr)
    - Condição START REPETIDA emitida pelo mestre.
    - É gerado emitindo outra condição START para iniciar uma nova transmissão.
    - A condição START REPEATED com endereço de dispositivo escravo (SLA) é emitida entre a condição START e STOP
    Função Repeated_Start - gera a condição REPEATED START para operação de leitura.
    Argumento de entrada: - possui argumento de entrada do endereço de leitura do dispositivo escravo (SLA + R).
    Retorno: - retorna o status do evento.
 */
uint8_t TWIMaster::Repeated_Start(char read_address){				/* TWI repeated start function */
	uint8_t status;											/* Declarar variável */
	TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);					/* Ative o TWI, gere condição de início e limpe o sinalizador de interrupção */
	while (!(TWCR & (1<<TWINT)));							/* Aguarde até que o TWI termine seu trabalho atual (condição inicial) */
	status = TWSR & 0xF8;									/* Leia o registro de status TWI com mascaramento de três bits inferiores */
	if (status != 0x10){									/* Verifique a condição de início repetido foi transmitida com sucesso ou não? */
        return 0;											/* Se não, retorne 0 para indicar falha de condição de início repetida */
	}
    TWDR = read_address;									/* Se sim, escreva SLA + R no registro de dados TWI */
	TWCR = (1<<TWEN)|(1<<TWINT);							/* Ative o TWI e limpe o sinalizador de interrupção */
	while (!(TWCR & (1<<TWINT)));							/* Aguarde até o TWI concluir seu trabalho atual (operação de gravação) */
	status = TWSR & 0xF8;									/* Leia o registro de status TWI com mascaramento de três bits inferiores */
	if (status == 0x40){									/* Verifique se o SLA + R foi transmitido e ack aceito ou não? */
        return 1;											/* Se sim, retorne 1 para indicar a confirmação recebida */ 
	}
    if (status == 0x20){									/* Verificar SLA + R foi transmitido e nack recebido ou não? */
        return 2;											/* Se sim, retorne 2 para indicar o nack recebido, ou seja, o dispositivo está ocupado */
	}else
        return 3;											/* Outro retorno 3 para indicar falha no SLA + R */
}

/*PARAR(P)
    - Evento STOP emitido pelo mestre para indicar que ele deve parar.
    - É gerado pela transição Baixa para Alta no SDA enquanto o SCL é Alto.
    Função Stop - inicia a condição STOP
    Argumento de entrada: - não possui argumento de entrada.
    Retorno: - não retorna nenhum tipo de dado.
 */
void TWIMaster::Stop(){											/* TWI stop function */
	TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);					/* Ative o TWI, gere a condição de parada e limpe o sinalizador de interrupção */
	while(TWCR & (1<<TWSTO));								/* Aguarde até a execução da condição de parada */
}

void TWIMaster::Start_Wait(char write_address){					/* I2C start wait function */
	uint8_t status;											/* Declarar variável */
	while (1)
	{
		TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);				/* Ative o TWI, gere condição de início e limpe o sinalizador de interrupção */
		while (!(TWCR & (1<<TWINT)));						/* Aguarde até que o TWI termine seu trabalho atual (condição inicial) */
		status = TWSR & 0xF8;								/* Leia o registro de status TWI com mascaramento de três bits inferiores */
		if (status != 0x08){								/* Verifique se a condição de início foi transmitida com sucesso ou não? */
            continue;										/* Se não, continue com o loop inicial novamente */
		}
        TWDR = write_address;								/* Se sim, escreva SLA + W no registro de dados TWI */
		TWCR = (1<<TWEN)|(1<<TWINT);						/* Ative o TWI e limpe o sinalizador de interrupção */
		while (!(TWCR & (1<<TWINT)));						/* Aguarde até o TWI concluir seu trabalho atual (operação de gravação) */
		status = TWSR & 0xF8;								/* Leia o registro de status TWI com mascaramento de três bits inferiores */
		if (status != 0x18 ){								/* Verifique se o SLA + W foi transmitido e o ack recebido ou não? */
			Stop();                                         /* Caso contrário, gere a condição de parada */
			continue;										/* Continue com o loop inicial novamente */
		}
		break;												/* Se sim, interrompa o loop */
	}
}

/*ESCREVA(W)
    - O evento WRITE data / address é emitido pelo mestre após o reconhecimento bem-sucedido do START recebido do dispositivo escravo.
    Função Write - grava dados/endereço no barramento
    Argumento de entrada: - possui argumento de entrada de dados / endereço.
    Retorno: - retorna o status do evento.
 */
uint8_t TWIMaster::Write(char data){								/* TWI write function */
	uint8_t status;											/* Declarar variável */
	TWDR = data;											/* Copiar dados no registro de dados TWI */
	TWCR = (1<<TWEN)|(1<<TWINT);							/* Ative o TWI e limpe o sinalizador de interrupção */
	while (!(TWCR & (1<<TWINT)));							/* Aguarde até o TWI concluir seu trabalho atual (operação de gravação) */
	status = TWSR & 0xF8;									/* Leia o registro de status TWI com mascaramento de três bits inferiores */
	if (status == 0x28){									/* Verifique se os dados foram transmitidos e ack recebido ou não? */
        return 0;											/* Se sim, retorne 0 para indicar que a confirmação foi recebida */
	}
    if (status == 0x30){									/* Verificar se os dados foram transmitidos e nack recebido ou não? */
        return 1;											/* Se sim, retorne 1 para indicar nack recebido */
	}else
        return 2;											/* Outro retorno 2 para indicar falha na transmissão de dados */
}

/*LER(R)
    - O evento de leitura de dados é emitido pelo mestre após a condição REPEATED START bem-sucedida.
    Função Read_Ack - lê dados disponíveis na linha SDA e retorna ao dispositivo escravo a confirmação sobre a leitura bem-sucedida dos dados,
    e também informa ao escravo para transmitir outros dados.
    Argumento de entrada: - não possui argumento de entrada.
    Retorno: - retorna dados recebidos.
 */
char TWIMaster::Read_Ack(){										/* TWI read ack function */
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA);					/* Ativar TWI, gerar o ack e limpar o sinalizador de interrupção */
	while (!(TWCR & (1<<TWINT)));							/* Aguarde até o TWI concluir seu trabalho atual (operação de leitura) */
	return TWDR;											/* Retornar dados recebidos */
}	

/*Função Read_Nack - lê o último byte de dados necessário disponível na linha SDA,mas não retorna o reconhecimento. 
    - Indica ao escravo que o mestre não deseja os próximos dados e deseja interromper a comunicação.
    Argumento de entrada: - não possui argumento de entrada.
    Retorno: - retorna os dados recebidos.
 */

char TWIMaster::Read_Nack(){										/* TWI read nack function */
	TWCR=(1<<TWEN)|(1<<TWINT);								/* Ative o TWI e limpe o sinalizador de interrupção */
	while (!(TWCR & (1<<TWINT)));							/* Aguarde até o TWI concluir seu trabalho atual (operação de leitura) */
	return TWDR;											/* Retornar dados recebidos */
}	
void TWIMaster::write(const char* data,uint8_t bytes, uint8_t addr,uint8_t reg) {
    Start_Wait(addr & ~1);              /* Inicia o I2C com o endereço de gravação do dispositivo */   
    Write(reg);                         /* Endereço de memória inicial para gravação de dados */
    for (int i = 0; i<bytes; i++){      /* Escrevendo array de dados */
        Write(data[i]); 
    }
    Stop();
}

void TWIMaster::receive(char* data,uint8_t bytes, uint8_t addr,uint8_t reg) {
    
    Start_Wait(addr & ~1);                   /* Inicia o I2C com o endereço de gravação do dispositivo */   
    Write(reg);                         /* Endereço de memória inicial para gravação de dados */
    Repeated_Start(addr | 1);
    for (int i = 0; i< bytes; i++){     /* Lendo os dados com ack */
        if(i == bytes -1){
            data[i] = Read_Nack();                        /* Lendo os dados com nack */
        }else{
            data[i] = Read_Ack();
        }
    }
    Stop();
}