import paho.mqtt.client as mqtt
import json

# Configurações MQTT
MQTT_BROKER = "broker.hivemq.com"  
MQTT_PORT = 1883
MQTT_TOPIC = "sensor/data"

def on_connect(client, userdata, flags, rc):
    print(f"Conectado ao broker MQTT com código de resultado: {rc}")
    client.subscribe(MQTT_TOPIC)
    print(f"Inscrito no tópico: {MQTT_TOPIC}")

def on_message(client, userdata, msg):
    try:
        payload = msg.payload.decode()
        data = json.loads(payload)
        
        tempC = data.get("temperatura")
        tensao = data.get("tensao")
        corrente = data.get("corrente")
        potencia = data.get("potencia")

        print(f"\nDados recebidos: ")
        print(f"  Temperatura: {tempC:.1f}°C")
        print(f"  Tensão: {tensao:.2f}V")
        print(f"  Corrente: {corrente:.2f}A")
        print(f"  Potência: {potencia:.1f}W")

        # Lógica para ações automáticas
        if tempC is not None:
            if tempC >= 50.0:
                print("  Ação: Temperatura acima de 50°C - Sistema de ventilação ativado.")
            elif tempC >= 35.0:
                print("  Ação: Temperatura acima de 35°C - Ventilador ligado.")
            elif tempC <= 32.0:
                print("  Ação: Temperatura abaixo de 32°C - Ventilador desligado.")
        
        if potencia is not None:
            if potencia <= 60.0:
                print("  Ação: Potência abaixo de 60W - Relé ativado.")
            elif potencia > 59.0:
                print("  Ação: Potência acima de 59W - Relé desativado.")

    except json.JSONDecodeError:
        print(f"Erro ao decodificar JSON: {msg.payload}")
    except Exception as e:
        print(f"Erro ao processar mensagem: {e}")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(MQTT_BROKER, MQTT_PORT, 60)

client.loop_forever()


