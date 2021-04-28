##  Programação Radiosonda Vaisala RS41 com IDE Arduino 
Biblioteca e IDE Arduino configurada para desenvolvimento de firmware para a Radiosonda Vaisala RS41

#### Informações sobre o Sistema
A IDE Arduino foi configurada no modo "portable" e foi criada uma placa "variant" com os parametros do SoC da RS41 (STM32F100C8T6).\
A biblioteca **vaisala_rs41.h**  implementa funções para acesso ao rádio da RS41 incluindo configuração da frequência de transmissão, habilitar, desabilitar transmissão;  comunicação com o GPS , serial  (XData port) , entre outras. Veja na seção exemplos a utilização das funções.  
Implementada função para transmissão de textos em RTTY.

#### Utilização  
- Faça o download do arquivo [arduino-1.8.13_stm32_radiosonda.zip](https://drive.google.com/file/d/1h-1w4kxMXWZvkXIx4UX6_UEwJW3LLON0/view?usp=sharing)
- Descompactar em uma pasta da sua escolha no sistema operacional Windows (testado com Windows 10).
- Executar o arquivo arduino.exe. 
- Faça uma verificação compilando alguns dos exemplos.

Para gravar o firmware é necessário ter o STlink e respectivo driver e software cliente instalado. 

#### Exemplos
Transmissão RTTY
```cpp
#include <SPI.h>
#include <vaisala_rs41.h>

char msg_tx_rtty[80];

void setup() {
  uint16_t leitura;   
  rs41_config_pinos_io(); 
  rs41_green_led (0);
  rs41_red_led (0); 
  
  Serial1.begin(9600); //GPS
  Serial3.begin(9600); 

  rs41_reset_gps ();
  
  radio_soft_reset();
  
  // configura frequencia de TX  
  radio_set_tx_frequency(403.0);
  
  //configura potencia de TX 
  radio_set_tx_power(1);
     
  radio_config_sensor_temp();  
  
  radio_enable_tx(); 
   
  delay(1000);  
}

void loop() {         
   sprintf(msg_tx_rtty,"Vaisala RS41 Programada com Arduino !");          
   radio_enable_tx();
   delay(50);  
   rtty_txstring (msg_tx_rtty);   
   delay(10);  
   radio_inhibit_tx();     
   delay(3000);       
   
}

```
Configuração de frequencia do rádio
```cpp
#include <SPI.h>
#include <vaisala_rs41.h>

void setup() {
  uint16_t leitura;   
  rs41_config_pinos_io(); 
  rs41_green_led (0);
  rs41_red_led (0); 
  
  Serial1.begin(9600); //GPS
  Serial3.begin(9600); 

  rs41_reset_gps ();
  
  radio_soft_reset();
  
  // configura frequencia de TX  
  radio_set_tx_frequency(403.0);
  
  //configura potencia de TX 
  radio_set_tx_power(1);
     
  radio_config_sensor_temp();  
  
  radio_enable_tx(); 
   
  delay(1000);  
}

void loop() {         
   radio_set_tx_frequency(403.0);  
   delay(2000); 
   radio_set_tx_frequency(402.8);  
   delay(2000); 
   radio_set_tx_frequency(403.3);  
   delay(2000);         
}
```
Monitorar o sensor de temperatura do rádio
```cpp
#include <SPI.h>
#include <vaisala_rs41.h>

void setup() {
  uint16_t leitura;   
  rs41_config_pinos_io(); 
  rs41_green_led (0);
  rs41_red_led (0); 
  
  Serial1.begin(9600); //GPS
  Serial3.begin(9600); 

  rs41_reset_gps ();
  
  radio_soft_reset();
  
  // configura frequencia de TX  
  radio_set_tx_frequency(403.0);
  
  //configura potencia de TX 
  radio_set_tx_power(1);
     
  radio_config_sensor_temp();  
  
  radio_enable_tx(); 
   
  delay(1000);  
}

void loop() {     
  Serial3.print ("temperatura: ");    
  Serial3.println (radio_ler_temperatura());    
}
```

Ler dados do GPS e exibir na Serial3
```cpp
#include <SPI.h>
#include <vaisala_rs41.h>

void setup() {
  uint16_t leitura;   
  rs41_config_pinos_io(); 
  rs41_green_led (0);
  rs41_red_led (0); 
  
  Serial1.begin(9600); //GPS
  Serial3.begin(9600); 

  rs41_reset_gps ();
  
  radio_soft_reset();
  
  // configura frequencia de TX  
  radio_set_tx_frequency(403.0);
  
  //configura potencia de TX 
  radio_set_tx_power(1);
     
  radio_config_sensor_temp();  
  
  radio_enable_tx(); 
   
  delay(1000);  
}

void loop() {     
   while (Serial1.available() > 0)
  {
    Serial3.write(Serial1.read());
  }

    
}
```

Acionamendo dos LED´s
```cpp
#include <SPI.h>
#include <vaisala_rs41.h>

void setup() {
  uint16_t leitura;   
  rs41_config_pinos_io(); 
  rs41_green_led (0);
  rs41_red_led (0); 
  
  Serial1.begin(9600); //GPS
  Serial3.begin(9600); 

  rs41_reset_gps ();
  
  radio_soft_reset();
  
  // configura frequencia de TX  
  radio_set_tx_frequency(403.0);
  
  //configura potencia de TX 
  radio_set_tx_power(1);
     
  radio_config_sensor_temp();  
  
  radio_enable_tx(); 
   
  delay(1000);  
}

void loop() {     
  rs41_green_led (1);
  rs41_red_led (0); 
  delay (1000);
  rs41_green_led (0);
  rs41_red_led (1); 
  delay(1000);
    
}
```








#### Contribuições e Agradecimentos
- https://projetoicaro.qsl.br/ 
- https://github.com/df8oe/RS41HUP
- https://github.com/bazjo/RS41_Hardware
- https://www.aeq-web.com/rs41-radiosonde-vaisala-firmware-flash-gps-aprs-om3bc/?lang=en
- https://ukhas.org.uk/guides:linkingarduinotontx2

