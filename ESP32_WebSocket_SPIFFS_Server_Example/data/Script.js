  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    //var state
    if (event.data == "led_green=1"){ document.getElementById('text_green').innerHTML = "ON"; document.getElementById('button_green').style.backgroundColor = "#00CC00"; }
    if (event.data == "led_red=1"){ document.getElementById('text_red').innerHTML = "ON"; document.getElementById('button_red').style.backgroundColor = "#CC0000"; }
    if (event.data == "led_blue=1"){ document.getElementById('text_blue').innerHTML = "ON"; document.getElementById('button_blue').style.backgroundColor = "#0000CC"; }
    if (event.data == "led_green=0"){ document.getElementById('text_green').innerHTML = "OFF"; document.getElementById('button_green').style.backgroundColor = "#888888"; }
    if (event.data == "led_red=0"){ document.getElementById('text_red').innerHTML = "OFF"; document.getElementById('button_red').style.backgroundColor = "#888888"; }
    if (event.data == "led_blue=0"){ document.getElementById('text_blue').innerHTML = "OFF"; document.getElementById('button_blue').style.backgroundColor = "#888888"; }
  }
  function onLoad(event) {
    initWebSocket();
    initButtons();
  }
  function initButtons() {
    if (document.getElementById('text_red').innerHTML == "ON") { document.getElementById('button_red').style.backgroundColor = "#CC0000"; }
    if (document.getElementById('text_green').innerHTML == "ON") { document.getElementById('button_green').style.backgroundColor = "#00CC00"; }
    if (document.getElementById('text_blue').innerHTML == "ON") { document.getElementById('button_blue').style.backgroundColor = "#0000CC"; }
    document.getElementById('button_red').addEventListener('click', fn_toggle_red);
    document.getElementById('button_green').addEventListener('click', fn_toggle_green);
    document.getElementById('button_blue').addEventListener('click', fn_toggle_blue);
  }
  function fn_toggle_red(){
    websocket.send('toggle_red');
  }
  function fn_toggle_green(){
    websocket.send('toggle_green');
  }
  function fn_toggle_blue(){
    websocket.send('toggle_blue');
  }
