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

#### Contribuições e Agradecimentos
- https://projetoicaro.qsl.br/ 
- https://github.com/df8oe/RS41HUP
- https://github.com/bazjo/RS41_Hardware
- https://www.aeq-web.com/rs41-radiosonde-vaisala-firmware-flash-gps-aprs-om3bc/?lang=en
- https://ukhas.org.uk/guides:linkingarduinotontx2

