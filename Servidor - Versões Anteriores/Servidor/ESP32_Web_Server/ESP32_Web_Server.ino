#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h> //https://github.com/bbx10/WebServer_tng

WebServer server ( 80 );
/*
  Utilizei o roteador do meu celular;
  Obs: não precisa ter internet =);
*/
const char* ssid     = "GustavoNr";
const char* password = "10101010";
int dataset[505];
int coordx[505];
int LEDPIN = 5;

String  ledState = "OFF";

void setup() 
{
  pinMode(LEDPIN, OUTPUT);
  
  Serial.begin(115200);

  connectToWifi();

  beginServer();
  
}

void loop() {
 
 server.handleClient();
 
 delay(1000);
 
}

/*
  connectToWifi():
  Conecta à rede passada pela string "ssid" com a senha "password", caso consiga conectar, imprime o valor aloca para ESP32 na rede indicada.

*/
void connectToWifi()
{
  WiFi.enableSTA(true);
  
  delay(2000);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {//Caso não consiga estabelecer a conexão.
        delay(500);
        Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
} 

/*
  beginServer():
  O código da função indica que quando uma requisição HTTP é recebida no caminho raiz (“/”), ela irá disparar a execução da função handleRoot;

*/
void beginServer()
{
  server.on ( "/", handleRoot );
  server.begin();
  Serial.println ( "HTTP server started" );
}

/*
    handleRoot():
    Verifica se o navegador adicionou o parâmetro name = "LED";
*/
void handleRoot(){ 
  for(int i = 0; i < 501;i++){  
      coordx[i] = i; 
      dataset[i] = f(i);
      server.send ( 200, "text/html", getPage() );//
  }
  
}

/*
  getSubmission():
  Pega o valor selecionado no no seletor radio;
*/
/*void getSubmission() {
  String LEDValue;
  LEDValue = server.arg("LED");//Pegando valor do seletor on/off
  Serial.println("Set GPIO "); 
  Serial.print(LEDValue);
  
  if ( LEDValue == "1" ) {
    digitalWrite(LEDPIN, HIGH);
    ledState = "On";
    server.send ( 200, "text/html", getPage() );
  }
  else if( LEDValue == "0" ) 
  {
    digitalWrite(LEDPIN, LOW);
    ledState = "Off";
    server.send ( 200, "text/html", getPage() );
  } else 
  {
    Serial.println("Error Led Value");
  }
}*/

/*
  getPage():
  Retorna o conteúdo da página;
*/
int f(int x){
   return x*x;
}

String getPage(){
  String page;
  page += "<!DOCTYPE html> <html> <head> <meta http-equiv='refresh' content='60'/> <meta charset=""utf-8""> <title>Plot Graph</title> <script src=""https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.8.0/Chart.bundle.js""></script> <style type=""text/css""> .ctxGraph{ display: block; position: relative;height:40vh;width:80vw;}.btnStop{position:relative;display:block;top:300px;left:50px;}</style> </head> <body> <div class=""ctxGraph""> <canvas class=""graph""></canvas></div>";
  page += "<script type=""text/javascript""> var ctx = document.getElementsByClassName(""graph""); var chartG = new Chart(ctx,{ type: 'line', data:{";
  //page += index;
  //page = +2;
  page += "datasets:[{label:""PID Graph""";
  //page += dataset;
  //page += 10;
  page += ", showLine: false, borderWhidth: 5, borderColor: 'rgba(0,0,200,.85)', backgroundColor: 'transparent', }] }, options:{ title:{ display: true, fontSize: 20, text: ""PID Control"", stacked: false, responsive: true, }, labels:{ fontStyle: ""bold"" }, scales:{ xAxes:[{ display: true, scaleLabel:{ display: true, fontColor: 'rgba(0,0,200,.85)', fontSize:12, fontStyle: ""bold"", labelString:'Time' } }], yAxes:[{ display: true, scaleLabel:{ display: true, fontColor: 'rgba(0,0,200,.85)', fontSize:12, fontStyle: ""bold"", labelString:'Distance' } }] } } }); </script> <input class= ""btnStop"" type=""button"" onclick=""clearInterval(timer)"" value=""Stop""></body> </html>";
        
  //String page = "<html lang=en-EN><head><meta http-equiv='refresh' content='60'/><title>ESP32 WebServer</title><style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }</style></head><body><h1>ESP32 WebServer</h1><h3>LED</h3><form action='/' method='POST'><ul><li>LED<INPUT type='radio' name='LED' value='1'>ON<INPUT type='radio' name='LED' value='0'>OFF</li></ul><INPUT type='submit' value='Submit'></body></html>";

  
  return page;
}
