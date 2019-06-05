const char WEB_PAGE[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="pt-br">
<!--Comentários-->
<head>
  <meta charset="UTF-8"/> 
  <title>PID Controller</title>
  
  <!--Incluindo Chart JS-->
  <script src = "https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.7.3/Chart.min.js"></script>  
  
  <style>
  canvas{
    -moz-user-select: none;
    -webkit-user-select: none;
    -ms-user-select: none;
  }
 
  /*Formatação da Tabela do Gráfico*/
  #chartDataTable {
    font-family: "Times New Roman", Times, serif;
    border-collapse: collapse;
    width: 100%;
  }
 
  #chartDataTable td, #chartDataTable th {
    border: 1px;
    border-color: rgb(128,128,128);
    border-style: solid;
    padding: 8px;
  }
 
  #chartDataTable tr:nth-child(even){background-color: #f2f2f2;}
  
  #chartDataTable th {
    padding-top: 12px;
    padding-bottom: 12px;
    text-align: left;
    background-color: rgba(0,0,200,.85);
    color: white;
  }

  /*Formatação da Tabela de Entradas*/
  #inputDataTable {
      font-family: "Times New Roman", Times, serif;
      border-collapse: collapse;
      width: 100%;
  }

  #inputDataTable td, #inputDataTable th {
    border: 1px;
    border-style: solid;
    border-color: rgb(128,128,128);
    padding: 10px;
  }

  #inputDataTable th {
    padding-top: 12px;
    padding-bottom: 12px;
    text-align: center;
    background-color: rgba(0,0,200,.85);
    color: white;
  }

  </style>
</head>
 
<body>
    <!--Tabela de Entradas-->
    <div id="inputData">
      <div style="text-align:center;"><b>Dados de Entrada</b></div>
      <table id="inputDataTable">
        <tr><th>Kp</th><th>Kd</th><th>Ki</th><th>Sinal de Referência</th><th>Status</th></tr> 
        <tr><td id="Kp">0</td><td id="Kd">0</td><td id="Ki">0</td><td id="refSignal">0</td><td id="status">0</td></tr> 
      </table>
    </div>

    <br/>
    
    <!--Gráfico-->
    <div style="text-align:center;"><b>Controller Graph</b><br></div>
    <div class="chart-container" position: relative; height:350px; width:100%">
        <canvas id="Chart" width="400" height="400"></canvas>
    </div>

  <!--Tabela do Gráfico-->
  <div>
    <table id="chartDataTable">
      <tr><th>Tempo</th><th>Distância</th></tr>
    </table>
  </div>
 
  <script>
  var txUpadate = 5000;//Taxa de atualização
  var distanceValues = [];
  var timeValues = [];
  function loadGraph(){

      var ctx = document.getElementById("Chart").getContext('2d');
      var Graph = new Chart(ctx, {
          type: 'line',
          data: {
              labels: timeValues,  //Eixo X (Distância)
              datasets: [{
                  label: "Distância",
                  fill: false,  
                  borderColor: 'rgba(0,0,200,.85)',// Cor da linha do Gráfico
                  backgroundColor: 'transparent',
                  data: distanceValues, //Eixo Y (Tempo)
              }],
          },
          options: {
              title: {
                      display: true,
                      text: "PID - f(time)"
                  },
              maintainAspectRatio: false,
              elements: {
              line: {
                      tension: 0.5 //Suavização das curvas
                  }
              },
              scales: {
                      xAxes:[{
                        display: true,
                        scaleLabel:{
                          display: true,
                          fontColor: 'rgba(0,0,200,.85)',
                          fontSize:12,
                          fontStyle: "bold",
                          labelString:'Tempo'    
                        }
                      }],
                      yAxes:[{
                        display: true,
                        ticks: { beginAtZero:true},
                        scaleLabel:{
                          display: true,
                          fontColor: 'rgba(0,0,200,.85)',
                          fontSize:12,
                          fontStyle: "bold",
                          labelString:'Distance'    
                        }
                      }]
              }
          }
      });
   
  }
   
  //Executa as funções após página ser carregada
  window.onload = function() {
    console.log(new Date().toLocaleTimeString());
    loadGraph();
  };
   
  //Chamada de getUrlData() de acordo com a taxa de atualização
  setInterval(function() {
    getUrlData();
    /*if(status != estabilizando){
      getUrlData();
    }
    else{
      if(status == estabilizou){
        //Saída....
      }
      else{
        //Saída....
      }
    }*/
  }, txUpadate); 
  

  function getUrlData() {
    var xhttp = new XMLHttpRequest();
    //Função que tratará a resposta do servidor
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        //Pegando JSON da URL 
        var jsonData = this.responseText;
        var data = JSON.parse(jsonData);

        //Inserindo valores novos dados nos vetores de dados.
        var timeAt = new Date().toLocaleTimeString();// Hora atual
        distanceValues.push(data.Distance);
        timeValues.push(timeAt);
        loadGraph();//Atualização do Gráfico
        
        //Valores de Entrada 
        document.getElementById("Kp").innerHTML = data.Kp;
        document.getElementById("Kd").innerHTML = data.Kd;
        document.getElementById("Ki").innerHTML = data.Ki;
        document.getElementById("refSignal").innerHTML = data.ReferenceSignal;//Sinal de Refência
        document.getElementById("status").innerHTML = data.SystemStatus;//Status do Sistema

        //Atualização da Tabela do Gráfico
        var table = document.getElementById("chartDataTable");
        var row = table.insertRow(1); //Adiciona nova linha na tabela
        var newCellT = row.insertCell(0); // Insere uma nova célula na linha
        var newCellD = row.insertCell(1); // Insere uma nova célula na linha
        newCellT.innerHTML = timeAt;
        newCellD.innerHTML = data.Distance;
      }
    };
    /*Parâmetros:
      1º Método GET para pegar o JSON via URL;
      2º Serviço(função) requisitada ao servidor;
      3º Comunicação Assíncrona(true).
    */
    xhttp.open("GET", "readRequest", true);  
    xhttp.send();
  }
      
  </script>
</body>
</html>

)=====";