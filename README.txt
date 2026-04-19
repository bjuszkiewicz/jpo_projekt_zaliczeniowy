Projekt: Generator maili z przypomnieniem o płatności
Wykorzystuje lokalny model LLM (Bielik) do generowania wiadomości na podstawie wprowadzonych danych. Posiada możliwość wyboru tonu wiadomości (Uprzejmy/Stanowczy/Ostateczny).

Instrukcja uruchomienia:
1. Upewnij się, że masz zainstalowany program Ollama.
2. Pobierz model wpisując w terminalu: ollama run SpeakLeash/bielik-11b-v3.0-instruct:Q4_K_M
3. Otwórz plik CMakeLists.txt w programie Qt Creator.
4. Upewnij się, że w tle działa program Ollama.
5. Skompiluj i uruchom projekt (zielony trójkąt).