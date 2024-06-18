#include <GyverINA.h>.
INA226 ina(0.01f, 1.0f, 0x40);

#include "GyverFilters.h"
GMedian<25, int> MedianFilter;
GFilterRA RAFilter;   // указываем тип данных в <>

#define CLK 5 //9
#define DIO 4 //10
#include "GyverTM1637.h"
GyverTM1637 disp(CLK, DIO);
uint32_t Now, clocktimer;
boolean flag;

float current = 0;
byte welcome_banner[] = {_H, _E, _L, _L, _O, _empty, _empty,
                         _n, _n, _y, _empty, _empty,
                         _F, _r, _e, _n, _d
                        };



void setup() {


  Serial.begin(115200);
  // установка коэффициента фильтрации (0.0... 1.0). Чем меньше, тем плавнее фильтр
  RAFilter.setCoef(0.05);
  // установка шага фильтрации (мс). Чем меньше, тем резче фильтр
  RAFilter.setStep(10);

  disp.clear();
  disp.brightness(7);  // яркость, 0 - 7 (минимум - максимум)
  disp.clear();
  //disp.runningString(welcome_banner, sizeof(welcome_banner), 300);  // выводим


  // Проверяем наличие и инициализируем INA226
  if (ina.begin()) {
    // Serial.println(F("Ina226_FC connected!"));
    ina.setSampleTime(INA226_VBUS, INA226_CONV_2116US);   // Повысим время выборки напряжения вдвое
    ina.setSampleTime(INA226_VSHUNT, INA226_CONV_8244US);// Напряжение в 12ти битном режиме + 64х кратное усреднение
    ina.setAveraging(INA226_AVG_X4); // Включим встроенное 4х кратное усреднение, по умолчанию усреднения нет
  } else {
    //  Serial.println(F("Ina226_FC not found!"));
    while (1);
  }

  // подключаем задачи (порядковый номер, имя функции, период в мс)


}

void loop() {
  current = ina.getPower() * 1000;
  Serial.print(current);
  Serial.print(",");
  Serial.print(MedianFilter.filtered(current));
  Serial.print(",");
  Serial.println(RAFilter.filtered(MedianFilter.filtered(current)));

  disp.displayInt(RAFilter.filtered(MedianFilter.filtered(current)));
  delay(50);

}
