int incomingByte;
void setup() {
    Serial.begin(9600); // скорость связи
} 
void loop (){
    if (Serial.available()>0)// вывод только при получении данных
    {
        incomingByte= Serial.read();//читаем байт из буфера
        Serial.write(incomingByte); //выводим байт в последовательный порт
    }
}