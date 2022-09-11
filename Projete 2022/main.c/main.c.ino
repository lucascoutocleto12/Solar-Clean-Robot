//Programa: Bot Telegram com ESP8266 NodeMCU

#include <Wire.h>  //INCLUSÃO DE BIBLIOTECA

#include "CTBot.h"
CTBot myBot;

//Definicoes da rede wifi e conexao
String ssid  = "ApLucas_2.4GHz"; //Nome da sua rede wifi
String pass  = "02092004"; //Senha da sua rede wifi
String token = ""; //Token bot Telegram

//Pinos
uint8_t led_wifi_stats = D4;
uint8_t relay_motorAgua = D3;
uint8_t relay_ventilador = D0;
uint8_t sensor1 = D7;
uint8_t sensor2 = D6;
uint8_t motor1Left = D5;
uint8_t motor1Right = D8;

const int LM35 = A0; // Define o pino que lera a saída do LM35
float temperatura; // Variável que armazenará a temperatura medida
float reservatorio1 = 0;
float reservatorio2 = 0;

void setup()
{

  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(LM35, INPUT);

  pinMode(motor1Left, OUTPUT);
  pinMode(motor1Right, OUTPUT);

  pinMode(led_wifi_stats, OUTPUT);

  pinMode(relay_motorAgua, OUTPUT);
  pinMode(relay_ventilador, OUTPUT);

  digitalWrite(led_on, LOW);

  Serial.begin(115200);
  
  Serial.println("Inicializando bot Telegram...");

  //Conexao na rede wifi
  myBot.wifiConnect(ssid, pass);

  //Define o token
  myBot.setTelegramToken(token);

  //Verifica a conexao
  if (myBot.testConnection())
  {
    Serial.println("Conexao Ok!");
    digitalWrite(led_wifi_stats, LOW);
  }
  else
    Serial.println("Falha na conexao!");
}

void loop()
{
  //temperatura = float((analogRead(LM35) * 330.0)/1023.0);
  float valor_analog_lm35 = float(analogRead(LM35)); 
  float tensao = (valor_analog_lm35 * 5) / 1023; 
  float temperatura = tensao / 0.010; 
  
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
 
  //Variavel que armazena a mensagem recebida
  TBMessage msg;

  digitalRead(sensor1);
  digitalRead(sensor2);


  //Verifica se chegou alguma mensagem
  if (myBot.getNewMessage(msg))
  {
    //Verifica se foi recebia a mensagem "Liga 1"
    if (msg.text.equalsIgnoreCase("reservatorio1"))
    {      
      //Retorna uma pensagem informando que o led foi aceso
      String reply;
      reply = (String) "Reservatorio de Agua: " +  "Nivel: ";
      if(sensor1 = 1) reply = reply + "Cheio";
      else reply + "Vazio";
      myBot.sendMessage(msg.sender.id, reply);
    }

    else if (msg.text.equalsIgnoreCase("reservatorio2"))
    {      
      //Retorna uma pensagem informando que o led foi aceso
      String reply;
      reply = (String) "Reservatorio de Sabao: " +  "Nivel: ";
      if(sensor2 = 1) reply = reply + "Cheio";
      else reply + "Vazio";
      myBot.sendMessage(msg.sender.id, reply);
    }

    else if (msg.text.equalsIgnoreCase("temperatura"))
    {      
      //Retorna uma pensagem informando que o led foi aceso
      String reply;
      reply = (String) "Temperatura: " + String(temperatura) + " C - Nivel: ";
      if(temperatura > 35) reply = reply + "ALERTA!";
      else if(temperatura > 31 && temperatura < 34) reply = reply + "Atencao!";
      else reply + "Normal.";
      myBot.sendMessage(msg.sender.id, reply);
    }

    else
    {
      //Caso receba qualquer outro comando, envia uma
      //mensagem generica/informativa
      String reply;
      reply = (String)"Desculpe. Comando Invalido.";
      myBot.sendMessage(msg.sender.id, reply);
    }

  }

}
