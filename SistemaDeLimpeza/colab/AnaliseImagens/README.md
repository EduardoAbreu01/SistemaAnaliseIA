# Análise de imagem para dizer se é necessário limpar ou não a placa.

Este projeto utiliza a API da OpenAI para analisar imagens de painéis solares e determinar:
 Se o painel precisa de limpeza.
 O nível de insolação (baixa, média ou alta).
 Uma análise detalhada da imagem.

# Funcionalidadess

- Análise automatizada de imagens de painéis solares.
- Classificação do nível de limpeza da placa (`needCleaning`).
- Categorização da radiação solar em três níveis:
  - `1` - Baixa
  - `2` - Média
  - `3` - Alta
- Geração de um JSON estruturado com os resultados.

# Tecnologias Utilizadas

- Python
- OpenAI GPT-4o Mini (API)
- Google Colab (com `userdata` para chave API)
- Base64 para codificação das imagens

# Estrutura do Código


- 1. Importação de bibliotecas
- 2. URLs das imagens (limpa, suja, ensolarada, nublada)
- 3. Função encode_image_to_base64() para codificar imagens
- 4. Classe VerificaPlaca para padronizar a resposta
- 5. Envio das imagens à API da OpenAI com instruções específicas
- 6. Impressão do JSON de resposta


# Como Usar

1. É necessário uma chave da OpenAI
2. Configure sua chave da OpenAI na variável de ambiente `userdata.get('OPENAI_API_KEY')` no Google Colab.
2. Execute o código fornecido para:
   - Baixar e codificar as imagens.
   - Enviar as imagens para a análise com o modelo da OpenAI.
   - Obter o JSON com os resultados e análises.
