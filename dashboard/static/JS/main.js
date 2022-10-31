const websocket = new WebSocket(
  'ws://'
  + window.location.host
  + '/dashboard/'
);
const servermessages = document.querySelector("[id = 'servermessages']")
const helloWorldButton = document.querySelector("#helloworld")
const dashboard = document.querySelector(".socketTest");

websocket.addEventListener("close", (e) => console.log(e))
websocket.addEventListener("open", (e) => console.log(e))
websocket.addEventListener("message", (e)=> {
	console.log(e)
	servermessages.insertAdjacentHTML("beforeend", `<br>${e.data}`)
})

if (helloWorldButton) {
	helloWorldButton.addEventListener("click", (e) => {
	console.log(e)
	websocket.send("hello world")
})}

