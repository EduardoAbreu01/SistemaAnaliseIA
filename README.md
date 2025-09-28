# 🌞 Challenge API – Análise de Painéis Solares

Este projeto é uma **API em FastAPI** para análise de imagens de **painéis solares** usando **IA (OpenAI GPT-4o-mini)**.  
O sistema recebe uma URL de imagem, verifica se o painel precisa de limpeza, classifica o nível de insolação e armazena os resultados em um banco de dados **MongoDB**.

---

## 🚀 Funcionalidades

- Autenticação via **Bearer Token**.  
- Integração com **OpenAI API** para análise automática de imagens.  
- Armazenamento dos relatórios no **MongoDB**.  
- Endpoints para:
  - Teste de conexão (`/challenge/`)
  - Listar análises realizadas (`/challenge/analises`)
  - Buscar análises por data (`/challenge/analises/data/{YYYY-MM-DD}`)
  - Realizar nova análise (`/challenge/analisar`)

---

## 📂 Estrutura do Projeto

```
app/
│── auth.py         # Autenticação
│── database.py     # Conexão com o MongoDB
│── llm_service.py  # Integração com OpenAI
│── main.py         # Configuração principal da API
│── models.py       # Definição dos modelos (Pydantic)
│── routes.py       # Definição das rotas da API
```

---

## 🛠️ Pré-requisitos

- Python **3.9+**
- Chave de API da OpenAI

---

## ⚙️ Instalação
Utilizando o terminal acesse o caminho \challenge-FIAP-main\challenge-FIAP-main
```bash
cd challenge-FIAP-main
```
Baixe as bibliotecas utilizando o arquivo **requirements.txt**
```bash
pip install -r requirements.txt
```

---

## 🔑 Variáveis de Ambiente

Crie um arquivo **.env** na raiz do projeto com:

```
API_TOKEN=seu_token_de_autenticacao
MONGO_URL=url_do_mongodb
OPENAI_API_KEY=sua_chave_openai
```

---

## ▶️ Como rodar
No terminal digite
```bash
uvicorn app.main:app --reload
```
## 📝 Resultado esperado
```bash
INFO:     Will watch for changes in these directories: ['C:\\Users\challenge-FIAP-main\\challenge-FIAP-main']
INFO:     Uvicorn running on http://127.0.0.1:8000 (Press CTRL+C to quit)
INFO:     Started reloader process [1172] using StatReload
INFO:     Started server process [5508]
INFO:     Waiting for application startup.
INFO:     Application startup complete.

```
A API estará disponível em:  
👉 [http://127.0.0.1:8000](http://127.0.0.1:8000)

---

## 📡 Endpoints

### 🔹 Testar API
```http
GET /challenge/
```

### 🔹 Listar análises
```http
GET /challenge/analises
```

### 🔹 Buscar análises por data
```http
GET /challenge/analises/data/{YYYY-MM-DD}
```

### 🔹 Realizar nova análise
```http
POST /challenge/analisar
```

**Body (JSON):**
```json
{
  "url": "https://exemplo.com/imagem-painel.jpg"
}
```

**Resposta:**
```json
{
  "status": "success",
  "url": "https://exemplo.com/imagem-painel.jpg",
  "need_cleaning": true,
  "solar_level": 2,
  "response": "O painel apresenta acúmulo de poeira...",
  "date_time": "2025-09-27T00:00:00Z"
}
```

---

## 🛡️ Segurança

- Todas as requisições exigem **Bearer Token** no header:

```
Authorization: Bearer <API_TOKEN>
```

---

## 📌 Tecnologias Utilizadas

- [FastAPI](https://fastapi.tiangolo.com/)  
- [MongoDB](https://www.mongodb.com/)  
- [OpenAI GPT-4o-mini](https://platform.openai.com/)  
- [Pydantic](https://docs.pydantic.dev/)  

---

## 👨‍💻 Autor
Projeto desenvolvido para fins acadêmicos na **Faculdade**, integrando conceitos de **IA + APIs + Banco de Dados**.  
