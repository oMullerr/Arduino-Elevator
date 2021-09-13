#include <Adafruit_NeoPixel.h> // incluimos a biblioteca para a fita de led
#define PIN 13                 // porque que a fita de led esta conectada
#define NUMPIXELS 10           // numero de leds

#define NEO_GRD // importamos

int ledOperante = 3;
int ledPortaAberta = 5;
int ledEmergencia = 4;
int botao = 2;
bool subindo = false;
bool descendo = false;
bool alinhado = true;
bool botoesDentro[] = {false, false, false, false, false, false, false, false, false, false};
bool botoesForaSobe[] = {false, false, false, false, false, false, false, false, false, false};
bool botoesForaDesce[] = {false, false, false, false, false, false, false, false, false, false};
bool estacionado = true;
bool operante = false;
bool emergencia = false;

int andarAtual = 0;

int delayval = 200;

int redColor = 0;
int greenColor = 0;
int blueColor = 0;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRD + NEO_KHZ800);

void setup()
{
  Serial.begin(9600); // inicia o serial
  pixels.begin();     // inicial as luzes

  pinMode(2, INPUT_PULLUP); // a interrupção esta ligada no pin 2
  attachInterrupt(botao - 2, int_botao, FALLING);
  pinMode(ledOperante, OUTPUT);    // led de operante
  pinMode(ledPortaAberta, OUTPUT); // led para mostrar que a porta do elevador esta ligada
  pinMode(ledEmergencia, OUTPUT);  // led de emergencias
}

void liga(int x)
{ // liga o led
  digitalWrite(x, HIGH);
}

void desliga(int x)
{ // desliga o led
  digitalWrite(x, LOW);
}

void setColor()
{ //definimos a cor do led
  redColor = random(0, 255);
  greenColor = random(0, 255);
  blueColor = random(0, 255);
}

void loop()
{
  liga(ledPortaAberta); // liga o led da porta aberta
  setColor();           // chamamos a função para definir a cor
  pixels.setPixelColor(andarAtual, pixels.Color(redColor, greenColor, blueColor));

  pixels.show(); // mostra a cor no led

  delay(delayval); // temos um delay

  pixels.setPixelColor(andarAtual, 0, 0, 0); // intensidade da cor
  if (operante)
  {
    liga(ledOperante);
    if (!descendo)
    {
      for (int i = 0; i < 10; i++)
      {
        if (botoesDentro[i] == true && i > andarAtual)
        {
          desliga(ledPortaAberta);
          subindo = true;
          estacionado = false;
          alinhado = false;
          for (andarAtual; andarAtual <= i; andarAtual++)
          {
            delay(500);
            Serial.println(andarAtual);
            alinhado = true;
            pixels.setPixelColor(andarAtual, pixels.Color(redColor, greenColor, blueColor));

            pixels.show();

            delay(delayval);

            pixels.setPixelColor(andarAtual, 0, 0, 0);
            //delay(delayval);

            while (emergencia)
            {
              subindo = false;
              delay(5000);
              break;
            }

            if (!emergencia)
            {
              operante = true;
            }

            if (botoesForaSobe[andarAtual])
            {
              Serial.println("entrou");
              liga(ledPortaAberta);
              subindo = false;
              botoesForaSobe[andarAtual] = false;
              delay(5000);
              break;
            }

            if (botoesDentro[andarAtual])
            {
              Serial.println("entrou");
              liga(ledPortaAberta);
              subindo = false;
              botoesDentro[andarAtual] = false;
              delay(5000);
              break;
            }
          }
        }
        if ((botoesForaSobe[i] or botoesForaDesce[i]) && i > andarAtual)
        {
          desliga(ledPortaAberta);
          subindo = true;
          estacionado = false;
          alinhado = false;
          if (botoesForaSobe[i])
          {
            for (andarAtual; andarAtual <= i; andarAtual++)
            {
              delay(500);
              Serial.println(andarAtual);
              alinhado = true;
              pixels.setPixelColor(andarAtual, pixels.Color(redColor, greenColor, blueColor));

              pixels.show();

              delay(delayval);

              pixels.setPixelColor(andarAtual, 0, 0, 0);
              //delay(delayval);

              while (emergencia)
              {
                subindo = false;
                delay(5000);
                break;
              }

              if (!emergencia)
              {
                operante = true;
              }

              if (botoesForaSobe[andarAtual])
              {
                Serial.println("entrou");
                liga(ledPortaAberta);
                subindo = false;
                botoesForaSobe[andarAtual] = false;
                delay(5000);
                for (int i = 0; i < 10; i++)
                {
                  if (botoesDentro[i] == true)
                  {
                    subindo = true;
                  }
                }
                break;
              }
            }
          }
          if (botoesForaDesce[i])
          {
            for (andarAtual; andarAtual <= i; andarAtual++)
            {
              delay(500);
              Serial.println(andarAtual);
              alinhado = true;

              pixels.setPixelColor(andarAtual, pixels.Color(redColor, greenColor, blueColor));
              pixels.show();
              delay(delayval);
              pixels.setPixelColor(andarAtual, 0, 0, 0);

              while (emergencia)
              {
                subindo = false;
                delay(5000);
                break;
              }

              if (!emergencia)
              {
                operante = true;
              }

              if (i == andarAtual)
              {
                Serial.println("entrou");
                liga(ledPortaAberta);
                subindo = false;
                botoesForaDesce[andarAtual] = false;
                delay(5000);
                for (int i = 0; i < 10; i++)
                {
                  if (botoesDentro[i] == true)
                  {
                    descendo = true;
                  }
                }
                break;
              }
            }
          }
        }
      }

      if (!subindo)
      {
        for (int i = 10; i > -1; i--)
        {
          if (botoesDentro[i] == true && i < andarAtual)
          {
            desliga(ledPortaAberta);
            descendo = true;
            estacionado = false;
            alinhado = false;
            for (andarAtual; andarAtual >= i; andarAtual--)
            {
              delay(500);
              Serial.println(andarAtual);
              alinhado = true;
              pixels.setPixelColor(andarAtual, pixels.Color(redColor, greenColor, blueColor));

              pixels.show();

              delay(delayval);

              pixels.setPixelColor(andarAtual, 0, 0, 0);
              //delay(delayval);

              while (emergencia)
              {
                subindo = false;
                delay(5000);
                break;
              }

              if (!emergencia)
              {
                operante = true;
              }

              if (botoesForaDesce[andarAtual])
              {
                Serial.println("entrou2");
                liga(ledPortaAberta);
                subindo = false;
                botoesForaDesce[andarAtual] = false;
                delay(5000);
                break;
              }

              if (botoesDentro[andarAtual])
              {
                Serial.println("entrou2");
                liga(ledPortaAberta);
                descendo = false;
                botoesDentro[andarAtual] = false;
                delay(5000);
                break;
              }

              if (andarAtual == i)
              {
                Serial.println("entrou2");
                liga(ledPortaAberta);
                descendo = false;
                botoesDentro[i] = false;
                delay(5000);
                break;
              }
            }
          }

          if ((botoesForaSobe[i] or botoesForaDesce[i]) && i < andarAtual)
          {
            desliga(ledPortaAberta);
            descendo = true;
            estacionado = false;
            alinhado = false;
            if (botoesForaSobe[i])
            {
              for (andarAtual; andarAtual >= i; andarAtual--)
              {
                delay(500);
                Serial.println(andarAtual);
                alinhado = true;
                pixels.setPixelColor(andarAtual, pixels.Color(redColor, greenColor, blueColor));

                pixels.show();

                delay(delayval);

                pixels.setPixelColor(andarAtual, 0, 0, 0);
                //delay(delayval);

                while (emergencia)
                {
                  subindo = false;
                  delay(5000);
                  break;
                }

                operante = true;

                if (i == andarAtual)
                {
                  Serial.println("entrou");
                  liga(ledPortaAberta);
                  descendo = false;
                  botoesForaSobe[andarAtual] = false;
                  delay(5000);
                  for (int i = 0; i < 10; i++)
                  {
                    if (botoesDentro[i] == true)
                    {
                      subindo = true;
                    }
                  }
                  break;
                }
              }
            }
            if (botoesForaDesce[i])
            {
              for (andarAtual; andarAtual >= i; andarAtual--)
              {
                delay(500);
                Serial.println(andarAtual);
                alinhado = true;
                pixels.setPixelColor(andarAtual, pixels.Color(redColor, greenColor, blueColor));

                pixels.show();

                delay(delayval);

                pixels.setPixelColor(andarAtual, 0, 0, 0);
                //delay(delayval);

                while (emergencia)
                {
                  subindo = false;
                  delay(5000);
                  break;
                }

                if (!emergencia)
                {
                  operante = true;
                }

                if (botoesForaDesce[andarAtual])
                {
                  Serial.println("entrou");
                  liga(ledPortaAberta);
                  descendo = false;
                  botoesForaDesce[andarAtual] = false;
                  delay(5000);
                  for (int i = 0; i < 10; i++)
                  {
                    if (botoesDentro[i] == true)
                    {
                      descendo = true;
                    }
                  }
                  break;
                }
              }
            }
          }
        }
      }
    }
  }
}
void int_botao()
{
  Serial.println("apertou!"); // mensagem que o botao foi apertado
  int v = analogRead(A0);

  switch (v)
  {
  case 214:
    operante = true;
    liga(ledOperante);
    break;
  case 227:
    operante = false;
    emergencia = false;
    desliga(ledPortaAberta);
    desliga(ledOperante);
    break;
  case 201:
    Serial.println("emergencia");
    if (emergencia == true)
    {
      emergencia = false;
      desliga(ledEmergencia);
    }

    else
    {
      emergencia = true;
      liga(ledEmergencia);
    }

    operante = false;
    desliga(ledOperante);
    break;
  case 20:
    botoesDentro[0] = true;
    break;
  case 40:
    botoesDentro[1] = true;
    break;
  case 59:
    botoesDentro[2] = true;
    break;
  case 77:
    botoesDentro[3] = true;
    break;
  case 95:
    botoesDentro[4] = true;
    break;
  case 112:
    botoesDentro[5] = true;
    break;
  case 128:
    botoesDentro[6] = true;
    break;
  case 144:
    botoesDentro[7] = true;
    break;
  case 159:
    botoesDentro[8] = true;
    break;
  case 173:
    botoesDentro[9] = true;
    break;
  case 336:
    botoesForaSobe[0] = true;
    break;
  case 346:
    botoesForaSobe[1] = true;
    break;
  case 355:
    botoesForaSobe[2] = true;
    break;
  case 363:
    botoesForaSobe[3] = true;
    break;
  case 372:
    botoesForaSobe[4] = true;
    break;
  case 380:
    botoesForaSobe[5] = true;
    break;
  case 388:
    botoesForaSobe[6] = true;
    break;
  case 396:
    botoesForaSobe[7] = true;
    break;
  case 404:
    botoesForaSobe[8] = true;
    break;
  case 240:
    botoesForaDesce[1] = true;
    break;
  case 252:
    botoesForaDesce[2] = true;
    break;
  case 263:
    botoesForaDesce[3] = true;
    break;
  case 275:
    botoesForaDesce[4] = true;
    break;
  case 286:
    botoesForaDesce[5] = true;
    break;
  case 296:
    botoesForaDesce[6] = true;
    break;
  case 307:
    botoesForaDesce[7] = true;
    break;
  case 317:
    botoesForaDesce[8] = true;
    break;
  case 327:
    botoesForaDesce[9] = true;
    break;
  }
}