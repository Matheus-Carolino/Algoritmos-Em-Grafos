# Algoritmos em Grafos
> Sistema de manipulação de grafos com interface CLI e leitura de arquivos

---

## Desenvolvedores
* **Matheus da Silva Carolino**
* **Matheus Henrique Ribeiro Cardoso**
* **Simão Baptista Sunga**
* **Pedro Tavares Oliveira**

---

## Estrutura do Projeto
* `/src`: Arquivos de código-fonte (`.c`)
* `/include`: Arquivos de cabeçalho (`.h`)
* `CMakeList.txt`: Arquivo de configuração de build (utilizado pelo CLion).

---

## Pré-requisitos
* **CLion** (ou outra IDE/editor de sua preferência com suporte a CMake)
* Um compilador C configurado (GCC, Clang ou MinGW)

---

### Clonar o Repositório
Abra o terminal na pasta onde deseja salvar o projeto e rode:
```bash
git clone https://github.com/Matheus-Carolino/Algoritmos-Em-Grafos.git
cd Algoritmos-Em-Grafos
```

### Opção 1: Via CLion (Recomendado para Desenvolvimento)
1. **Abrir projeto**: Clique em **Open** no CLion e selecione a pasta raiz.
2. **Carregar CMake**: Aguarde a leitura automática do `CMakeLists.txt`.
3. **Executar**: Clique no botão **Run** na interface da IDE.

### Opção 2: Via Terminal (Windows)
1. **Compilar código**: Execute o GCC apontando para todos os arquivos fontes.
   ```cmd
   gcc src\*.c -I include -o meu_programa.exe
   ```
2. **Rodar binário**: Execute o arquivo gerado diretamente no Prompt ou PowerShell.
   ```cmd
   .\meu_programa.exe
   ```

### Opção 3: Via Terminal (Linux)
1. **Compilar código**: Use as barras padrões do Linux para compilar as fontes.
   ```bash
   gcc src/*.c -I include -o meu_programa
   ```
2. **Rodar binário**: Execute o programa gerado no diretório atual.
   ```bash
   ./meu_programa
   ```