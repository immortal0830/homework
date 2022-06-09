#include <IRremote.h> //IR 리모컨 라이브러리.
#include <Stepper.h>

const int stepsPerRevolution = 2048;

Stepper myStepper(stepsPerRevolution, 3, 4, 5, 6);

IRrecv irrecv(2); // IR리모컨을 사용하기 위해 7번핀 사용 선언
decode_results results; //IR리모컨으로부터 받은 신홋값

void setup()
{
    Serial.begin(9600); //시리얼 모니터 사용
    irrecv.enableIRIn(); // 리모컨 수신 사용
    myStepper.setSpeed(14);
}
void loop() {
    if (irrecv.decode(&results)==true) // 리모컨으로부터 받은 신호가 있으면
    {
      if(results.value == 0xFFA857)
      {
        myStepper.step(stepsPerRevolution);
      }
      else if(results.value == 0xFFE01F)
      {
        myStepper.step(-stepsPerRevolution);
      }
        Serial.println(results.value,HEX); //받은 신호를 시리얼 모니터에 입력
        irrecv.resume(); // 다음 신호 수신
    }
    int sensorValue = analogRead(A0);
    if(sensorValue >= 200)
    {
      myStepper.step(stepsPerRevolution);
    }
    Serial.println(sensorValue);
    delay(200);
}
