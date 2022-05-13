const path = require('path');
const express = require('express');
const WebSocket = require('ws');
const { start } = require('repl');
const app = express();

const WS_PORT  = 65080;
const HTTP_PORT = 80;
try{
const wsServer = new WebSocket.Server({port: WS_PORT}, ()=> console.log(`WS Server is listening at ${WS_PORT}`));

let connectedClients = [];
wsServer.on('connection', (uws, req)=>{
    console.log('Connected');
    connectedClients.push(uws);

    uws.on('message', data => {
        connectedClients.forEach((uws,i)=>{
            if(uws.readyState === uws.OPEN){
                uws.send(data);
            }else{
                connectedClients.splice(i ,1);
            }
        })
    });
});

app.get('/client',(req,res)=>res.sendFile(path.resolve(__dirname, './client.html')));
app.listen(HTTP_PORT, ()=> console.log(`HTTP server listening at ${HTTP_PORT}`));
}
catch (error) {
    console.error(error);
    // expected output: ReferenceError: nonExistentFunction is not defined
    // Note - error messages will vary depending on browser
  }