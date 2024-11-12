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
let gameFinished = false;

let gameState;

engine.stdout.on('data', (data) => {
    let engineData = `${data}`;
    console.log(engineData);
    let index = engineData.indexOf('_X_');
    if (index != -1){
        gameState = engineData.substring(index, index + 75);
    }
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

engine.on('exit', (program) => {
    if (program === 0){
        console.log('SUUIIIII');
    } else {
        console.log('NOOOO');
    }
    gameFinished = true;
})

app.use(express.text());

app.post('/move', (req, res) => {
    const move = req.body.trim();
    console.log(move);
    engine.stdin.write(move + '\n');
    setTimeout(() => {
        if (gameFinished){
            res.status(200).send('Game is finished');
        } else {
            if (moveSuccess){
                console.log("Succccccess");
                res.status(200).send('Move success' + engineMove);
            } else {
                console.log("NNPOOOOOO");
                res.status(200).send('Move fail');
            }
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