int button = 2;
int led = 8;
void setup() {
    pinMode(led, OUTPUT); // объявляем пин 8 как выход
    pinMode(button, INPUT); // объявляем пин 2 как вход
} 
void loop (){
    if (digitalRead(button) == HIGH) { // проверка получения сигнала
        digitalWrite(led, HIGH); // зажигаем светодиод
    } else {
        digitalWrite(led, LOW); // выключаем светодиод
    }
}
