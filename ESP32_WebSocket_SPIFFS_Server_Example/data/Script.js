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
    if (event.data == "GPIO12=1"){ document.getElementById('state12').innerHTML = "ON"; document.getElementById('button12').style.backgroundColor = "#00CC00"; }
    if (event.data == "GPIO13=1"){ document.getElementById('state13').innerHTML = "ON"; document.getElementById('button13').style.backgroundColor = "#CC0000"; }
    if (event.data == "GPIO14=1"){ document.getElementById('state14').innerHTML = "ON"; document.getElementById('button14').style.backgroundColor = "#0000CC"; }
    if (event.data == "GPIO12=0"){ document.getElementById('state12').innerHTML = "OFF"; document.getElementById('button12').style.backgroundColor = "#888888"; }
    if (event.data == "GPIO13=0"){ document.getElementById('state13').innerHTML = "OFF"; document.getElementById('button13').style.backgroundColor = "#888888"; }
    if (event.data == "GPIO14=0"){ document.getElementById('state14').innerHTML = "OFF"; document.getElementById('button14').style.backgroundColor = "#888888"; }
  }
  function onLoad(event) {
    initWebSocket();
    initButtons();
  }
  function initButtons() {
    document.getElementById('button12').addEventListener('click', toggle12);
    document.getElementById('button13').addEventListener('click', toggle13);
    document.getElementById('button14').addEventListener('click', toggle14);
  }
  function toggle12(){
    websocket.send('toggle12');
  }
  function toggle13(){
    websocket.send('toggle13');
  }
  function toggle14(){
    websocket.send('toggle14');
  }
