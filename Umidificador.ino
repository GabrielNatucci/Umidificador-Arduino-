#include <MicroLCD.h> // Essa biblioteca controla o monitor lcd oled de uma forma simples.
#include <DHT.h> // Biblioteca do sensor de humidade.

int relay = 2;
int i = 0;
int u = 0;
unsigned long int tempo = 0;

LCD_SSD1306 lcd; // Neste caso, escrevi LCD_SSD1306 pois esse é o microprocessador presente no meu monitor oled. Sendo assim, pode variar de acordo com o modelo usado.
DHT dht(A3, DHT11); // "A3" é o pino que eu conectei o sensor de umidade e o "DHT11" é o modelo dele.

void setup() 
  {
    pinMode(A3,INPUT);
    pinMode(relay,OUTPUT);
    dht.begin(); // Começa comunicação com o sensor de umidade.
    lcd.begin(); // Começa comunicação com o display oled lcd.
  }

void display() // Criei essa função para no "void loop" o código ficar mais legível.
  {
    if (millis() - tempo > 500) // Neste caso, preferi usar a função "millis" no lugar da "delay" pois assim,
                                // o display pode funcionar em paralelo do restante do código.
      {
      float humidade = dht.readHumidity(); // Esse comando puxa o valor de humidade do sensor, e aplica ele na variável "humidade".
      if (humidade<80)
        {
          lcd.setCursor(33, 0);
          lcd.println("Ligado!");
        }
      else
        {
          lcd.setCursor(20, 0);
          lcd.println("Desligado!");
        }
      lcd.setCursor(25, 3);
      lcd.println("Humidade:");
      lcd.setCursor(36, 6);
      lcd.println(humidade);
      lcd.setCursor(83, 6);
      lcd.println("%");
      tempo = millis();
      }
  }
  
void troca() // Essa função tem como objetivo trocar o estado do umidificador, fechando e abrindo o circuito, da mesma forma que um usuário faria apertando o botão do dispositivo.
  {
    digitalWrite(relay,HIGH);
    delay(50);
    digitalWrite(relay,LOW);
  }

void loop() 
  {
    lcd.setFontSize(FONT_SIZE_MEDIUM); // Esse comando seleciona o tamanho da fonte como médio. Neste tamanho, as letras cabem perfeitamente para esse uso.
    float humidade = dht.readHumidity(); // Esse comando puxa o valor de humidade do sensor, e aplica ele na variável "humidade".
    if (humidade<80)
      {
        lcd.clear();
        i = 0;  // Abaixo, usei a variável "i" para fazer com que o circuito só troque o estado do umidificador na primeira vez que o código passar por esse "while". 
                // Por isso é preciso zerar o valor de i aqui.
        while(humidade<80)
          {
            display(); // O display atualiza aqui.
            if(i==0)
              {
                troca();
                u = 1;
                i++; // Esse comando "i++", faz o arduino não executar esse "if" depois de ser executado pela primeira vez.
              }
            humidade = dht.readHumidity(); // Lê o valor de humidade de novo, para verificar se o código precisa sair desse loop.
          }
      }
    else
      {
      lcd.clear();
      i = 0;
      while(humidade>=80)
        {
        display();
        if(i==0 and u>0) // Nesse "if" coloquei esse "u" também porque se a humidade já estiver acima de 80%, ele não tente trocar o estado do umidificador de primeira.
                         // Se isso acontecesse, o umidificador acabaria ligando quando não deveria. Por isso, o código só entrar nesse "if", 
                         // quando ele ligar pela primeira vez. Na linha 69 tem um "u++", aí o u passa a ser maior que zero.
          {
            troca();
            i++;
          }
        humidade = dht.readHumidity(); // Lê o valor de humidade de novo, para verificar se o código precisa sair desse loop.
        }
      }
  }
