/*PROGRAMA REALIZADO PARA VILTEC CON LA FINALIDAD DE TENER UN MEJOR CONTROL DEL PERSONAL EN EL TALLER
FECHA: ABRIL DE 2018
PROGRAMA REALIZADO POR: CESAR ZABDIEL CONTRERAS ARIAS*/
#include <SPI.h>                                       //LIBRERIAS NECESARIAS PARA INICIAR EL MODULO RFID, SE ESTA CONSIDERANDO 
#include <MFRC522.h>                                  //RETIRAR LA BIBLIOTECA SERVO, YA QUE NO SE VA A UTILIZAR ESTA LIBRRERIA              
#include <Wire.h>                                   //CONFIGURACION DE LA LCD PARA SU FUNCIONAMIENTO
#include <LiquidCrystal_I2C.h>                     //USO DEL PROTOCOLO I2C PARA OPTIMIZAR ESPACIO EN EL CASE DEL SISTEMA
LiquidCrystal_I2C lcd(0x27,20,4);                 //A veces la dirección no es 0x3f. Cambia a 0x27 si no funciona.
#define RST_PIN  9                               //PIN 9  ES EL RESET DEL MODULO RFID
#define SS_PIN  10                              //PIN 10 ES EL SDA   DEL MODULO RFID 
MFRC522 mfrc522(SS_PIN, RST_PIN); //CREA UN NUEVO OBJETO PARA EL MODULO 
//Variables
boolean door_opened=false;
boolean first_read=false;
boolean normal_mode=true;
boolean countdown = false;
int timer=0;
int user_added=0;
int  add_ID_counter = 0;
//ENTRADAS Y SALIDAS EXTRAS
int buzzer_pin=7;
int chapa_pin=2;
int outside_close = 4;      //pin for the button of the outside open
int inside_close = 6;       //pin for the button of the inside close
int inside_open = 5;        //pin for the button of the inside open
/* YA SE DEFINIERON TODAS LAS VARIABLES NECESARIAS PARA LA REALIZACION DEL PROYECTO CON CONTADOR Y CON EL IDENTIFICADOR 
*  DE LA PUERTA PRINCIPAL DEL TALLER, VERIFICAR QUE ESTE COMPILANDO, COMO PRIMER PASO:
*  1.- VERIFICAR QUE LA TARJETA MAESTRA FUNCIONA CORRECTAMENTE
*  2.- QUE ABRA Y CIEERE SIN PROBLEMAS
*  3.- QUE PUEDA HACER EL REGIISTRO DEL PERSONAL
*  4.- QUE REALICE EL CONTEO- TEMPORIZADO DEL TIEMPO
*/
void setup() {
SPI.begin();                 //INICIA UN NUEVO SPI 
mfrc522.PCD_Init();          // Startt the MFRC522  
//  motor_1.digitalWrite(3);           //Set digital pin D3 to be the PWM signal for the servo motor
pinMode(buzzer_pin,OUTPUT);  //Set digital pin D7 to be the buzzer OUTPUT
pinMode(chapa_pin,OUTPUT);
//Config of the i2c LCD screen
lcd.init();
lcd.init();
lcd.backlight();
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Show your");
lcd.setCursor(3,3);
lcd.print("ID");
digitalWrite(buzzer_pin,HIGH);
digitalWrite(chapa_pin,HIGH);//Activo el MOTOR DC //Put the servo in to the close position
}
byte ActualUID[4];                               //Esto almacenará la identificación cada vez que lea un nuevo código de identificación.
byte USER1[4]= {0x02, 0x10, 0xD1, 0xB5} ;       //Código de ID maestro. Cámbielo por su etiqueta. Primero use el ejemplo LEER y verifique su ID
byte USER2[4]= {0x50, 0x70, 0x5F, 0x56} ;      //Empty ID of USER2 Cesar CONTRERAS   
byte USER3[4]= {0x62, 0xAA, 0x49, 0x1A} ;   //Empty ID of USER5
byte USER4[4]= {0x52, 0x71, 0x22, 0x1A} ;   //Empty ID of USER5
byte USER5[4]= {0xE1, 0x40, 0xBC, 0x1C} ;   //Empty ID of USER5
byte USER6[4]= {0xE1, 0x93, 0xA8, 0x1C} ;   //Empty ID of USER5
byte USER7[4]= {0x62, 0x2E, 0x69, 0x1A} ;   //Empty ID of USER5
byte USER8[4]= {0x62, 0xFF, 0x13, 0x1A} ;   //Empty ID of USER5
byte USER9[4]= {0xF1, 0x68, 0xB9, 0x1C} ;   //Empty ID of USER5
byte USER10[4]= {0xE1, 0x04, 0xE4, 0x1C} ;   //Empty ID of USER5
byte USER11[4]= {0xE1, 0x45, 0x25, 0x1C} ;   //Empty ID of USER5  
/* mover posicion de usuarios para agregar más a la lista de usuarios*/
//SUBRUTINA PARA CERRAR LA PUERTA DESDE AFUERA
void loop() {
if(digitalRead(outside_close))        //If the button is pressed, we close the door OUTSIDE PIN 4
{
door_opened=false;
first_read=false;
countdown = false;
add_ID_counter = 0;
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Welcome to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(-1,2);
lcd.print("Close Door");
lcd.setCursor(1,3);
lcd.print("Wait");  // puerta cerrada desde afuera 
digitalWrite(chapa_pin,HIGH);
digitalWrite(buzzer_pin,LOW);
delay(1500);
digitalWrite(chapa_pin,HIGH);
digitalWrite(buzzer_pin,HIGH);
delay(1000);
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Welcome to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Show Your");
lcd.setCursor(3,3);
lcd.print("ID");
}
if(digitalRead(inside_close))         //If the button is pressed, we close the door INSIDE  PIN 6 
{
door_opened=false; 
first_read=false;
countdown = false;
add_ID_counter = 0; 
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear 
lcd.setCursor(2,0);
lcd.print("Welcome to"); /////////////////////////////
lcd.setCursor(2,1);
lcd.print("Arduino Day");  ///puerta cerrada
lcd.setCursor(-1,2);
lcd.print("Close door");
lcd.setCursor(3,3);
lcd.print("Wait");
digitalWrite(chapa_pin,HIGH);
digitalWrite(buzzer_pin,LOW);
delay(2000);
digitalWrite(buzzer_pin,HIGH);
digitalWrite(chapa_pin,HIGH);
delay(1000);
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Welcome to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Show you");
lcd.setCursor(3,3);
lcd.print("ID");
}
if(digitalRead(inside_open))        //If the button is pressed, we open the door  PIN 5
{
door_opened=true;
first_read=false;
countdown = false;
add_ID_counter = 0;
digitalWrite(chapa_pin,HIGH);
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Welcome to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(-1,2);
lcd.print("Door open");   
lcd.setCursor(2,3);
lcd.print("Wait");
// digitalWrite(buzzer_pin,LOW);
digitalWrite(chapa_pin,LOW);
delay(1000);
// digitalWrite(buzzer_pin,HIGH);
digitalWrite(chapa_pin,HIGH);
delay(1000);
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Welcome to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Show your");
lcd.setCursor(3,3);
lcd.print("ID");
}
if(normal_mode)
{
if(countdown)
{
if(add_ID_counter > 300)
{
countdown = false;
first_read=false; 
add_ID_counter = 0;
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Arduino Day");
lcd.setCursor(4,1);
lcd.print("New Id");
lcd.setCursor(5,2);
lcd.print("Cancell");
lcd.setCursor(-2,3);
lcd.print("Show your ID");
digitalWrite(buzzer_pin,HIGH);
delay(1000);
digitalWrite(buzzer_pin,HIGH);
delay(4000);
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Welcome to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Show your");
lcd.setCursor(3,3);
lcd.print("ID");
}
if(add_ID_counter == 50)
{
lcd.setCursor(4,3);
lcd.print("5");
digitalWrite(buzzer_pin,HIGH);
delay(100);
digitalWrite(buzzer_pin,HIGH);
}
if(add_ID_counter == 100)
{
lcd.setCursor(4,3);
lcd.print("4");
digitalWrite(buzzer_pin,HIGH);
delay(100);
digitalWrite(buzzer_pin,HIGH);
}
if(add_ID_counter == 150)
{
lcd.setCursor(4,3);
lcd.print("3");
digitalWrite(buzzer_pin,HIGH);
delay(100);
digitalWrite(buzzer_pin,HIGH);
}
if(add_ID_counter == 200)
{
lcd.setCursor(4,3);
lcd.print("2");
digitalWrite(buzzer_pin,HIGH);
delay(100);
digitalWrite(buzzer_pin,HIGH);
}
if(add_ID_counter == 250)
{
lcd.setCursor(4,3);
lcd.print("1");
digitalWrite(buzzer_pin,HIGH);
delay(100);
digitalWrite(buzzer_pin,HIGH);
}   
add_ID_counter = add_ID_counter+1;
delay(10);
}   
// Compruebe si hay alguna nueva tarjeta de identificación delante del sensor
if ( mfrc522.PICC_IsNewCardPresent()) 
{  
//Select the found card
if ( mfrc522.PICC_ReadCardSerial()) 
{
// Almacenamos el ID de lectura en 4 bytes con un bucle for               
for (byte i = 0; i < mfrc522.uid.size; i++) {
ActualUID[i]=mfrc522.uid.uidByte[i];          
}                            
//Compare el UID y verifique si el nuevo ID está en la lista de usuarios                 
if(first_read)
{
if(compareArray(ActualUID,USER1))
{ 
countdown=false;
add_ID_counter = 0;
digitalWrite(buzzer_pin,HIGH);
delay(300);
digitalWrite(buzzer_pin,LOW);
normal_mode=false;
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Arduino Day");
lcd.setCursor(0,1);
lcd.print("Show new ID");
lcd.setCursor(2,2);
lcd.print("Wait");
lcd.setCursor(1,3);
lcd.print("Loading");
digitalWrite(buzzer_pin,HIGH);
delay(100);
digitalWrite(buzzer_pin,LOW);                                        
delay(1000);lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(0,0);
lcd.print("Arduino Day");
lcd.setCursor(0,1);
lcd.print("Show new ID");
lcd.setCursor(2,2);
lcd.print("Wait");
lcd.setCursor(1,3);
lcd.print("Loading");
digitalWrite(buzzer_pin,HIGH);
delay(100);
digitalWrite(buzzer_pin,LOW);
delay(1000);
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(0,0);
lcd.print("Arduino Day");
lcd.setCursor(0,1);
lcd.print("Show new ID");
lcd.setCursor(2,2);
lcd.print("Wait");
lcd.setCursor(1,3);
lcd.print("Loading");

digitalWrite(buzzer_pin,HIGH);
delay(100);
digitalWrite(buzzer_pin,LOW);
delay(1000);
lcd.clear();
lcd.setCursor(2,0);
lcd.print("Arduino Day");
lcd.setCursor(2,1);
lcd.print(" !!!!!NOW!!!!!!!");
lcd.setCursor(2,2);
lcd.print("Wait");
lcd.setCursor(1,3);
lcd.print("Loading");
digitalWrite(buzzer_pin,HIGH);
delay(500);
digitalWrite(buzzer_pin,LOW);                                
}
else
{
first_read=false;                    
}
}      
if(!first_read)
{
if(compareArray(ActualUID,USER1))//////////////////////////////////////////////////////////////////////////////////////////
{
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Arduino Day");
lcd.setCursor(2,1);
lcd.print("<Access>");
lcd.setCursor(0,2);
lcd.print("Master Card");
lcd.setCursor(1,3);
lcd.print("Show your ID");
digitalWrite(buzzer_pin,LOW);    //use analogWrite(buzzer_pin,200); for non active buzzer
digitalWrite(chapa_pin,LOW);
delay(1000);
digitalWrite(buzzer_pin,HIGH);    //use analog write for non active buzzer
digitalWrite(chapa_pin,HIGH);
door_opened=true;
first_read=true;
countdown=true;
delay(2000);
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Put new Card");
lcd.setCursor(2,1);
lcd.print("New ID");
digitalWrite(buzzer_pin,HIGH);
delay(100);
digitalWrite(buzzer_pin,HIGH);                                  
}
else if(compareArray(ActualUID,USER2))
{
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Arduino Day");
lcd.setCursor(2,1);
lcd.print("<Access>");
lcd.setCursor(0,2);
lcd.print("Cesar contreras");
lcd.setCursor(0,3);
lcd.print("Welcome");
digitalWrite(buzzer_pin,LOW);    //use analogWrite(buzzer_pin,200); for non active buzzer
digitalWrite(chapa_pin,LOW);
delay(1000);
digitalWrite(buzzer_pin,HIGH);    //use analog write for non active buzzer
digitalWrite(chapa_pin,HIGH);
door_opened=true;
first_read=true;
delay(3000);
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Wait");
lcd.setCursor(3,3);
lcd.print(".-.");
delay(3000);      
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Show your");
lcd.setCursor(3,3);
lcd.print("ID");
}
else if(compareArray(ActualUID,USER3))
{
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Arduino Day");
lcd.setCursor(2,1);
lcd.print("Authorized access");
lcd.setCursor(0,2);
lcd.print("User 3");
lcd.setCursor(0,3);
lcd.print("Welcome");
digitalWrite(buzzer_pin,LOW);    //use analogWrite(buzzer_pin,200); for non active buzzer
digitalWrite(chapa_pin,LOW);
delay(1000);
digitalWrite(buzzer_pin,HIGH);    //use analog write for non active buzzer
digitalWrite(chapa_pin,HIGH);
door_opened=true;
first_read=true;
delay(3000);
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Wait");
lcd.setCursor(3,3);
lcd.print("._.");
delay(3000);
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Show your");
lcd.setCursor(3,3);
lcd.print("ID");
}
else if(compareArray(ActualUID,USER4))
{
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Arduino Day");
lcd.setCursor(2,1);
lcd.print("Authorized access");
lcd.setCursor(0,2);
lcd.print("User 4");
lcd.setCursor(0,3);
lcd.print("Welcome");
digitalWrite(buzzer_pin,LOW);    //use analogWrite(buzzer_pin,200); for non active buzzer
digitalWrite(chapa_pin,LOW);
delay(1000);
digitalWrite(buzzer_pin,HIGH);    //use analog write for non active buzzer
digitalWrite(chapa_pin,HIGH);
door_opened=true;
first_read=true;
delay(3000);
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Wait");
lcd.setCursor(3,3);
lcd.print("._.");
delay(3000);
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Show your");
lcd.setCursor(3,3);
lcd.print("ID");
}
else if(compareArray(ActualUID,USER5))
{
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Arduino Day");
lcd.setCursor(2,1);
lcd.print("Authorized access");
lcd.setCursor(0,2);
lcd.print("User 5");
lcd.setCursor(0,3);
lcd.print("Welcome");
digitalWrite(buzzer_pin,LOW);    //use analogWrite(buzzer_pin,200); for non active buzzer
digitalWrite(chapa_pin,LOW);
delay(1000);
digitalWrite(buzzer_pin,HIGH);    //use analog write for non active buzzer
digitalWrite(chapa_pin,HIGH);
door_opened=true;
first_read=true;
delay(3000);
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Wait");
lcd.setCursor(3,3);
lcd.print("._.");
delay(3000);
lcd.clear();
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Show your");
lcd.setCursor(3,3);
lcd.print("ID");
}
else if(compareArray(ActualUID,USER6))
{
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Arduino Day");
lcd.setCursor(2,1);
lcd.print("Authorized access");
lcd.setCursor(0,2);
lcd.print("User 6");
lcd.setCursor(0,3);
lcd.print("Welcome");
digitalWrite(buzzer_pin,LOW);    //use analogWrite(buzzer_pin,200); for non active buzzer
digitalWrite(chapa_pin,LOW);
delay(1000);
digitalWrite(buzzer_pin,HIGH);    //use analog write for non active buzzer
digitalWrite(chapa_pin,HIGH);
door_opened=true;
first_read=true;
delay(3000);
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Wait");
lcd.setCursor(3,3);
lcd.print("._.");
delay(3000);
lcd.clear();
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Show your");
lcd.setCursor(3,3);
lcd.print("ID");
}
else if(compareArray(ActualUID,USER7))
{
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Arduino Day");
lcd.setCursor(2,1);
lcd.print("Authorized access");
lcd.setCursor(0,2);
lcd.print("User 7");
lcd.setCursor(0,3);
lcd.print("Welcome");
digitalWrite(buzzer_pin,LOW);    //use analogWrite(buzzer_pin,200); for non active buzzer
digitalWrite(chapa_pin,LOW);
delay(1000);
digitalWrite(buzzer_pin,HIGH);    //use analog write for non active buzzer
digitalWrite(chapa_pin,HIGH);
door_opened=true;
first_read=true;
delay(3000);
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Wait");
lcd.setCursor(3,3);
lcd.print("._.");
delay(3000);
lcd.clear();
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Show your");
lcd.setCursor(3,3);
lcd.print("ID");
}
else if(compareArray(ActualUID,USER8))
{
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Arduino Day");
lcd.setCursor(2,1);
lcd.print("Authorized access");
lcd.setCursor(0,2);
lcd.print("User 8");
lcd.setCursor(0,3);
lcd.print("Welcome");
digitalWrite(buzzer_pin,LOW);    //use analogWrite(buzzer_pin,200); for non active buzzer
digitalWrite(chapa_pin,LOW);
delay(1000);
digitalWrite(buzzer_pin,HIGH);    //use analog write for non active buzzer
digitalWrite(chapa_pin,HIGH);
door_opened=true;
first_read=true;
delay(3000);
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Wait");
lcd.setCursor(3,3);
lcd.print("._.");
delay(3000);
lcd.clear();
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Show your");
lcd.setCursor(3,3);
lcd.print("ID");
}
else if(compareArray(ActualUID,USER9))
{
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Arduino Day");
lcd.setCursor(2,1);
lcd.print("Authorized access");
lcd.setCursor(0,2);
lcd.print("User 9");
lcd.setCursor(0,3);
lcd.print("Welcome");
digitalWrite(buzzer_pin,LOW);    //use analogWrite(buzzer_pin,200); for non active buzzer
digitalWrite(chapa_pin,LOW);
delay(1000);
digitalWrite(buzzer_pin,HIGH);    //use analog write for non active buzzer
digitalWrite(chapa_pin,HIGH);
door_opened=true;
first_read=true;
delay(3000);
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Wait");
lcd.setCursor(3,3);
lcd.print("._.");
delay(3000);
lcd.clear();
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Show your");
lcd.setCursor(3,3);
lcd.print("ID");
}
else if(compareArray(ActualUID,USER10))
{
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Arduino Day");
lcd.setCursor(2,1);
lcd.print("Authorized access");
lcd.setCursor(0,2);
lcd.print("User 10");
lcd.setCursor(0,3);
lcd.print("Welcome");
digitalWrite(buzzer_pin,LOW);    //use analogWrite(buzzer_pin,200); for non active buzzer
digitalWrite(chapa_pin,LOW);
delay(1000);
digitalWrite(buzzer_pin,HIGH);    //use analog write for non active buzzer
digitalWrite(chapa_pin,HIGH);
door_opened=true;
first_read=true;
delay(3000);
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Wait");
lcd.setCursor(3,3);
lcd.print("._.");
delay(3000);
lcd.clear();
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Show your");
lcd.setCursor(3,3);
lcd.print("ID");
}
else
{
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(0,0);
lcd.print("Arduino Day");
lcd.setCursor(2,1);
lcd.print("Access denied");
lcd.setCursor(2,2);
lcd.print(" ID Unknow");
lcd.setCursor(1,3);
lcd.print("Show Your ID");
digitalWrite(buzzer_pin,LOW);    //use analog write for non active buzzer
delay(800);
digitalWrite(buzzer_pin, HIGH);    //use analog write for non active buzzer
delay(800);
door_opened=false;
first_read=false;
delay(3000);
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(2,0);
lcd.print("Welcome  to");
lcd.setCursor(2,1);
lcd.print("Arduino Day");
lcd.setCursor(0,2);
lcd.print("Show your");
lcd.setCursor(3,3);
lcd.print("ID");
}
}                              
// Terminamos la lectura de la tarjeta tarjeta  actual
mfrc522.PICC_HaltA();                  
}
}
}//end  normal mode
if(!normal_mode)
{
// Revisamos si hay nuevas tarjetas  presentes
if ( mfrc522.PICC_IsNewCardPresent()) 
{  
//Seleccionamos una tarjeta
if ( mfrc522.PICC_ReadCardSerial()) 
{
// Enviamos serialemente su UID
for (byte i = 0; i < mfrc522.uid.size; i++) {
ActualUID[i]=mfrc522.uid.uidByte[i];          
} 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                  
//Compare the read ID and the stored USERS
if(user_added==4)
{
lcd.setCursor(0,0);
lcd.print("LISTA   USUARIOS");
lcd.setCursor(0,1);
lcd.print("      LLENA       "); 
digitalWrite(buzzer_pin,HIGH);    //use analogWrite(buzzer_pin,200); for non active buzzer
delay(3000);
digitalWrite(buzzer_pin,LOW);    //use analog write for non active buzzer                     
lcd.setCursor(0,0);
lcd.print("Arduino Day ");
lcd.setCursor(0,1);
lcd.print("Aguascalientes ");
} 
if(user_added==3)
{
USER5[0] = ActualUID[0];
USER5[1] = ActualUID[1];
USER5[2] = ActualUID[2];
USER5[3] = ActualUID[3];  
user_added = user_added+1;
lcd.setCursor(0,0);
lcd.print("New user");
lcd.setCursor(0,1);
lcd.print("   as  USER 5   ");
digitalWrite(buzzer_pin,HIGH);    //use analogWrite(buzzer_pin,200); for non active buzzer
delay(300);
digitalWrite(buzzer_pin,LOW);    //use analog write for non active buzzer
delay(1000);
normal_mode=true;
first_read=false;
delay(3000);
lcd.setCursor(0,0);
lcd.print("Place card here!");
lcd.setCursor(0,1);
lcd.print("                ");
}      
if(user_added==2)
{
USER4[0] = ActualUID[0];
USER4[1] = ActualUID[1];
USER4[2] = ActualUID[2];
USER4[3] = ActualUID[3];  
user_added = user_added+1;
lcd.setCursor(0,0);
lcd.print("New user stored ");
lcd.setCursor(0,1);
lcd.print("   as  USER 4   "); 
digitalWrite(buzzer_pin,HIGH);    //use analogWrite(buzzer_pin,200); for non active buzzer
delay(300);
digitalWrite(buzzer_pin,LOW);    //use analog write for non active buzzer
delay(1000);
normal_mode=true;
first_read=false;
delay(3000);
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(0,0);
lcd.print("Place card here!");
lcd.setCursor(0,1);
lcd.print("                ");
}             
if(user_added==1)
{
USER3[0] = ActualUID[0];
USER3[1] = ActualUID[1];
USER3[2] = ActualUID[2];
USER3[3] = ActualUID[3];  
user_added = user_added+1;
lcd.setCursor(0,0);
lcd.print("New user stored ");
lcd.setCursor(0,1);
lcd.print("   as  USER 3   "); 
digitalWrite(buzzer_pin,HIGH);    //use analogWrite(buzzer_pin,200); for non active buzzer
delay(300);
digitalWrite(buzzer_pin,LOW);    //use analog write for non active buzzer
delay(1000);
normal_mode=true;
first_read=false;
delay(3000);
lcd.setCursor(0,0);
lcd.print("Place card here!");
lcd.setCursor(0,1);
lcd.print("                ");
}   
if(user_added==0)
{
USER2[0] = ActualUID[0];
USER2[1] = ActualUID[1];
USER2[2] = ActualUID[2];
USER2[3] = ActualUID[3];  
user_added = user_added+1;
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(0,0);
lcd.print("New user stored ");
lcd.setCursor(0,1);
lcd.print("   as  USER 2   ");
digitalWrite(buzzer_pin,HIGH);    //use analogWrite(buzzer_pin,200); for non active buzzer
delay(300);
digitalWrite(buzzer_pin,LOW);    //use analog write for non active buzzer
delay(1000);
normal_mode=true;
first_read=false;
delay(3000);
lcd.clear();                       //////////////////////////////////////////////////////////////////////////////////////////////////////////LCD clear
lcd.setCursor(0,0);
lcd.print("Place card here!");
lcd.setCursor(0,1);
lcd.print("                ");
}                     
}     
}
}//end  ID add mode
}
//Compare the 4 bytes of the users and the received ID
boolean compareArray(byte array1[],byte array2[])
{
if(array1[0] != array2[0])return(false);
if(array1[1] != array2[1])return(false);
if(array1[2] != array2[2])return(false);
if(array1[3] != array2[3])return(false);
return(true);
}
