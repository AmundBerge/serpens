const express = require('express');
const path = require('path');
const app = express();
const port = 3000;

const { spawn } = require('child_process');

const engine = spawn('./chess');

let moveSuccess = true;
let infoSuccess = true;
let playingAgainstEngine = false;
let engineMove = '';

engine.stdout.on('data', (data) => {
    let engineData = `${data}`;
    console.log('here comes the data', '\n' + engineData);
    moveSuccess = !engineData.includes('failed');
    infoSuccess = engineData.includes('Playing against engine');
    if (infoSuccess){
        playingAgainstEngine = true;
    }
    if (playingAgainstEngine){
        if (engineData.includes('Engine move')){
            let index = engineData.indexOf('Engine move');
            engineMove = engineData.substring(index + 13, index + 17);
        }

    }
})

app.use(express.text());

app.post('/move', (req, res) => {
    const move = req.body.trim();
    engine.stdin.write(move + '\n');
    setTimeout(() => {
        if (moveSuccess){
            res.status(200).send('Move success' + engineMove);
        } else {
            res.status(200).send('Move fail');
        }
    }, 100);
})

app.post('/misc', (req, res) => {
    const info = req.body.trim();
    engine.stdin.write(info + '\n');
    setTimeout(() => {
        if (infoSuccess){
            res.status(200).send('Info success');
        } else {
            res.status(200).send('Info fail');
        }
    }, 100);
})

app.use(express.static(path.join(__dirname, 'public')));

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'views', 'index.html'));
})

app.listen(port, () => {
    console.log("YURRRR");
})