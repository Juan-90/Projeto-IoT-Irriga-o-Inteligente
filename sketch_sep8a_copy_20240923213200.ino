#define BLYNK_PRINT Serial  // Habilita a impressão de mensagens do Blynk
#define BLYNK_TEMPLATE_ID "TMPL2CCpeiaRD"
#define BLYNK_TEMPLATE_NAME "Irrigação Inteligente"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Definições dos pinos
#define RELAY_PIN 14           // Pino que controla o Relé
#define SOIL_SENSOR_PIN 34     // Pino do Sensor de Umidade do Solo (analógico)

// Informações de rede Wi-Fi e Blynk
char auth[] = "0HaPfhfMMGIi0bwC7zjs88NRp5F4C2uA";  // Token de autenticação Blynk
char ssid[] = "LOVANI";                 // Nome da rede Wi-Fi
char pass[] = "LOCOMOTIVA";             // Senha da rede Wi-Fi

// Variáveis globais
int soilMoistureValue = 0;  // Armazena a leitura do sensor de umidade

// Função de configuração inicial
void setup() {
  // Inicializa a comunicação serial
  Serial.begin(9600);
  
  // Configura os pinos
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Mantém o relé desligado inicialmente

  // Inicializa a conexão com o Blynk
  Blynk.begin(auth, ssid, pass);

  // Mensagem de status de inicialização
  Serial.println("Sistema de Irrigação Inteligente Iniciado");
}

// Função principal de loop
void loop() {
    if (Blynk.connected()) {
        Blynk.run();
    } else {
        Serial.println("Tentando reconectar ao Blynk...");
        Blynk.connect();
        delay(5000); // Aguarda 5 segundos antes de tentar novamente
    }
    verificaUmidade();
}


// Função para verificar a umidade do solo e controlar o relé
void verificaUmidade() {
  // Leitura do sensor de umidade do solo
  soilMoistureValue = analogRead(SOIL_SENSOR_PIN);

  // Converte o valor lido para uma escala percentual (0% a 100%)
  int moisturePercent = map(soilMoistureValue, 4095, 0, 0, 100);

  // Exibe a leitura no serial
  Serial.print("Umidade do Solo: ");
  Serial.print(moisturePercent);
  Serial.println("%");

  // Controle automático da bomba baseado na umidade do solo
  if (moisturePercent < 20) {  // Limite ajustável de umidade
    digitalWrite(RELAY_PIN, LOW);  // Liga a bomba
    Serial.println("Bomba Ligada - Umidade Baixa");
    Blynk.virtualWrite(V1, 1);  // Atualiza o status do botão no Blynk (opcional)
  } else {
    digitalWrite(RELAY_PIN, HIGH);  // Desliga a bomba
    Serial.println("Bomba Desligada - Umidade Adequada");
    Blynk.virtualWrite(V1, 0);  // Atualiza o status do botão no Blynk (opcional)
  }
}

// Função que permite controle manual da bomba pelo Blynk (Botão no Virtual Pin V1)
BLYNK_WRITE(V1) {
  int buttonState = param.asInt();  // Lê o estado do botão (0 ou 1)
  
  if (buttonState == 1) {
    digitalWrite(RELAY_PIN, LOW);  // Liga a bomba
    Serial.println("Bomba Ligada Manualmente pelo Blynk");
  } else {
    digitalWrite(RELAY_PIN, HIGH);  // Desliga a bomba
    Serial.println("Bomba Desligada Manualmente pelo Blynk");
  }
}
