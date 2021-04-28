#define RTTY_DEVIATION 0x2  // RTTY shift = RTTY_DEVIATION x 270Hz

HardwareSerial Serial3(PB11, PB10); //USART 3
HardwareSerial Serial1(PA10, PA9); //GPS

//                MOSI  MISO  SCLK
SPIClass SPIRadio(PB15, PB14, PB13);

static const uint8_t WR = 0x80;
static const float SI4032_CLOCK = 26.0;

uint8_t radio_spi_transfer(const uint8_t register_addr, uint8_t value, uint8_t write){  
  uint16_t palavra_16bits = ((write ? register_addr | WR : register_addr) << 8) | value;     
  SPIRadio.beginTransaction(SPISettings(1000000 , MSBFIRST, SPI_MODE_3));   
  digitalWrite(PC13, LOW);   
  uint16_t leitura = SPIRadio.transfer16(palavra_16bits);      
  digitalWrite(PC13, HIGH); 
  SPIRadio.endTransaction();      
  return leitura ;
}


void radio_set_tx_frequency(const float freq_in_mhz) {
  uint8_t hbsel = (uint8_t) ((freq_in_mhz * (30.0f / SI4032_CLOCK)) >= 480.0f ? 1 : 0);
  uint8_t fb = (uint8_t) ((((uint8_t)((freq_in_mhz * (30.0f / SI4032_CLOCK)) / 10) - 24) - (24 * hbsel)) / (1 + hbsel));
  uint8_t gen_div  =  3;  // constant - not possible to change!
  uint16_t fc = (uint16_t) (((freq_in_mhz / ((SI4032_CLOCK / gen_div) * (hbsel + 1))) - fb - 24) * 64000);
  radio_spi_transfer(0x75, (uint8_t) (0b01000000 | (fb & 0b11111) | ((hbsel & 0b1) << 5)), 1);
  radio_spi_transfer(0x76, (uint8_t) (((uint16_t)fc >> 8) & 0xff), 1);
  radio_spi_transfer(0x77, (uint8_t) ((uint16_t)fc & 0xff), 1);
}

void radio_disable_tx() {
  radio_spi_transfer(0x07, 0x40, 1);
}

void radio_soft_reset() {
  radio_spi_transfer(0x07, 0x80, 1);
}

void radio_enable_tx() {
  // Modified to set the PLL and Crystal enable bits to high. Not sure if this makes much differents.
  radio_spi_transfer(0x07, 0x4B, 1); 
}

void radio_inhibit_tx() {
  // Sleep mode, but with PLL idle mode enabled, in an attempt to reduce drift on key-up.
  radio_spi_transfer(0x07, 0x43, 1);
}

uint8_t radio_ler_temperatura() {
   uint8_t temp_reg= radio_spi_transfer(0x11 , 0xff, 0);   
   uint8_t tempC= ((int8_t)(-64 + (temp_reg * 5 / 10) - 16));
   radio_spi_transfer(0x0f , 0x80, 1); 
   return tempC;
 }

void radio_config_sensor_temp() {
  radio_spi_transfer(0x13 , 0xf0, 1); // Temperature Value Offset
  radio_spi_transfer(0x12 , 0x00, 1); // Temperature Sensor Calibration
  radio_spi_transfer(0x0f , 0x80, 1); // ADC configuration
}

void radio_set_tx_power (uint8_t potencia) {
  // 0 --> -1dBm
  // 1 --> 2dBm
  // 2 --> 5dBm
  // 3 --> 8dBm
  // 4 --> 11dBm
  // 5 --> 14dBm
  // 6 --> 17dBm
  // 7 --> 20dBm
  radio_spi_transfer(0x6D, 00 | (potencia & 0x0007), 1);
}

void rs41_config_pinos_io(){
  pinMode(PC13, OUTPUT); // chip select SPI
  pinMode(PB8, OUTPUT);  // red led
  pinMode(PB7, OUTPUT);  // green led
  pinMode(PA15, OUTPUT);  //Reset GPS 
}

void rs41_green_led (uint8_t val ) {   
digitalWrite(PB7, !val); 
}

void rs41_red_led (uint8_t val ) {   
digitalWrite(PB8, !val); 
}

void rs41_reset_gps () {   
 digitalWrite(PA15, LOW);  
 delay(1);
 digitalWrite(PA15, HIGH);   
}





void rtty_txbit (int bit)
{
  if (bit) //bit 1
  {    
    radio_spi_transfer(0x73, RTTY_DEVIATION, 1);   
  }
  else //bit 0
  {
    radio_spi_transfer(0x73, 0x00, 1);  
    
  } 
//delayMicroseconds(3370); // 300 baud
  delayMicroseconds(10000); // For 50 Baud uncomment this and the line below. 
  delayMicroseconds(10150);     
}

void rtty_txbyte (char c)
{
  /* Simple function to sent each bit of a char to 
   	** rtty_txbit function. 
   	** NB The bits are sent Least Significant Bit first
   	**
   	** All chars should be preceded with a 0 and 
   	** proceded with a 1. 0 = Start bit; 1 = Stop bit
   	**
   	*/ 
  int i; 
  rtty_txbit (0); // Start bit 
  // Send bits for for char LSB first	 
  for (i=0;i<7;i++) // Change this here 7 or 8 for ASCII-7 / ASCII-8
  {
    if (c & 1) rtty_txbit(1);  
    else rtty_txbit(0);	 
    c = c >> 1; 
  } 
  rtty_txbit (1); // Stop bit
  rtty_txbit (1); // Stop bit
}



void rtty_txstring (char * string)
{
 //Each char is one byte (8 Bits) 
  char c; 
  c = *string++; 
  while ( c != '\0')
  {
    rtty_txbyte (c);
    c = *string++;
  }
}





