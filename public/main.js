let squaresList = [];

let startSquare = '';
let endSquare = '';

let moveSuccess = false; 
let playingAgainstEngine = false;
let engineMove = '';

let letters = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'];
for (let i = 0; i < letters.length; i++){
    let letter = letters[i];
    for (let j = 1; j <= 8; j++){
        let square = document.querySelector('#' + letter + j);
        square.addEventListener('click', function(){
            if (startSquare && endSquare){
                startSquare = "";
                endSquare = "";
            } else if (startSquare && !endSquare){
                endSquare = square.id;
            } else {
                startSquare = square.id;
            }
            updateStatus();
        })
        squaresList.push(square);
    }
}

let board = document.querySelector('#board');
let statusElement = document.querySelector('#status');

function updateStatus(){
    statusElement.innerText = `Start square: ${startSquare} | End square: ${endSquare}`;
}

async function updateBoard(){
    if (!(startSquare && endSquare)){
        return;
    }
    let start = document.querySelector(`#${startSquare}`);
    let end = document.querySelector(`#${endSquare}`);
    await sendMove(start.id + end.id);
    console.log(moveSuccess);
    if (moveSuccess){
        updatePieces(start.id + end.id);
        if (playingAgainstEngine){
            updatePieces(engineMove);
        }
    } 
    moveSuccess = false;
}

function updatePieces(move){
    if (move.length != 4){
        console.log('Error in move length');
        return;
    }
    let start = document.querySelector(`#${move.substring(0, 2)}`);
    let end = document.querySelector(`#${move.substring(2, 4)}`);
    let pieceImage = window.getComputedStyle(start).backgroundImage;
    start.style.backgroundImage = 'none';
    end.style.backgroundImage = pieceImage;
}

async function updateInfo(){
    const infoField = document.querySelector('#info-field');
    const text = infoField.value;
    sendInfo(text);
}

document.addEventListener('click', function(event){
    if (!board.contains(event.target)){
        startSquare = "";
        endSquare = "";
    }
    updateStatus();
})

async function sendMove(move){
    try {
        const response = await fetch('/move', {
            method: 'POST', 
            headers: {
                'Content-Type': 'text/plain'
            },
            body: move + '\n'
        })
        if (response.ok){
            const result = await response.text();
            console.log(result);
            if (result.length > 14){
                engineMove = result.substring(result.length - 4, result.length);
                console.log(engineMove, 'yes');
            }
            moveSuccess = (result.includes('Move success'));
        }
    } catch (error){
        console.log(error, 'errorrrrr');
    } 
}   

async function sendInfo(info){
    try {
        const response = await fetch('/misc', {
            method: 'POST', 
            headers: {
                'Content-Type': 'text/plain'
            },
            body: info + '\n'
        })
        if (response.ok){
            const result = await response.text();
            console.log(result, 'hello');
            playingAgainstEngine = true;
        }
    } catch (error){
        console.log(error, 'error info');
    } 
}