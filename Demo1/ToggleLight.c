void setup() {
    pinMode(13, OUTPUT); // объявляем пин 13 как выход
}
void loop (){
    digitalWrite(13, HIGH);// зажигаем светодиод
    delay(1000); // ждём 1 секунду
    digitalWrite(13, LOW); // выключаем светодиод
    delay(1000); // ждём 1 секунду
}