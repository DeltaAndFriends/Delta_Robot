#include "Gyro.h"

std::vector<Gyro> m_gyros;
double alpha;
double AlphaMin = 20000000;
double AlphaMax = -2000000;

void setup(){
m_gyros.push_back(Gyro{GYRO_PIN::_1});
for (auto g : m_gyros)
  {
    g.setup();
  }

	Serial.begin(115200);
}

void loop(){
	for (size_t i = 0; i < m_gyros.size(); ++i) {
      m_gyros.at(i).read();
		alpha = atan2(-1*(m_gyros.at(i).getacc(GD::Zacc), m_gyros.at(i).getacc(GD::Xacc)))*180/3.14159265;
		Serial.print(alpha);
		Serial.println(",");
   }
}

void _loop(){
   for (size_t i = 0; i < m_gyros.size(); ++i) {
      m_gyros.at(i).read();
		Serial.print("Accelerometer X: ");
		Serial.print(m_gyros.at(i).get(GD::Xacc));
		Serial.print(" Accelerometer Y: ");
		Serial.print(m_gyros.at(i).get(GD::Yacc));
		Serial.print(" Accelerometer Z: ");
		Serial.print(m_gyros.at(i).get(GD::Zacc));
		Serial.print(" Total value: ");
		Serial.print(sqrt(m_gyros.at(i).get(GD::Xacc)*m_gyros.at(i).get(GD::Xacc)+m_gyros.at(i).get(GD::Yacc)*m_gyros.at(i).get(GD::Yacc)+m_gyros.at(i).get(GD::Zacc)*m_gyros.at(i).get(GD::Zacc)));\
		Serial.print(" Alpha: ");
		alpha = atan(-1*(m_gyros.at(i).get(GD::Zacc)/m_gyros.at(i).get(GD::Xacc)))*180/3.14159265;
		Serial.print(alpha);
		Serial.print(" AlphaMin: ");
		Serial.print((alpha < AlphaMin)? (AlphaMin = alpha) : AlphaMin);
		Serial.print(" AlphaMax: ");
		Serial.print((alpha > AlphaMax) ? (AlphaMax = alpha) : AlphaMax);
		Serial.println();
   }

}
