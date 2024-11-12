let squaresList = [];

let startSquare = '';
let endSquare = '';

// let moveSuccess = false; 
let playingAgainstEngine = false;
let engineMove = '';

const startingBoard = "_X_RNBQKBNRPPPPPPPP□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□pppppppprnbqkbnr-X-W1111";

function placePieces(str){
    if (str.length != 75){
        console.log('wrong string format');
        return;
    }
    let squares = str.substring(3, 67);
    for (let i = 0; i < 64; i++){

        let myLetter = letters[i % 8];
        let myNumber = Math.floor(i / 8) + 1;
        /* let myLetter = letters[Math.floor(i / 8)];
        let myNumber = i % 8 + 1; */
        console.log(myLetter, myNumber);
        let character = squares[i];
        let square = document.querySelector('#' + myLetter + myNumber);
        switch (character){
            case '□':
                square.style.backgroundImage = 'none';
                break;
            case 'P':
                square.style.backgroundImage = "url('./resources/wP.svg')";
                break;
            case 'p':
                square.style.backgroundImage = "url('./resources/bP.svg')";
                break;
            case 'N':
                square.style.backgroundImage = "url('./resources/wN.svg')";
                break;
            case 'n':
                square.style.backgroundImage = "url('./resources/bN.svg')";
                break;
            case 'B':
                square.style.backgroundImage = "url('./resources/wB.svg')";
                break;
            case 'b':
                square.style.backgroundImage = "url('./resources/bB.svg')";
                break;
            case 'R':
                square.style.backgroundImage = "url('./resources/wR.svg')";
                break;
            case 'r':
                square.style.backgroundImage = "url('./resources/bR.svg')";
                break;
            case 'Q':
                square.style.backgroundImage = "url('./resources/wQ.svg')";
                break;
            case 'q':
                square.style.backgroundImage = "url('./resources/bQ.svg')";
                break;
            case 'K':
                square.style.backgroundImage = "url('./resources/wK.svg')";
                break;
            case 'k':
                square.style.backgroundImage = "url('./resources/bK.svg')";
                break;
            default:
                console.log("ERROR: ", character);
                break;
        }
    }
}

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
   /*  if (moveSuccess){
        updatePieces(start.id + end.id);
        if (playingAgainstEngine){
            updatePieces(engineMove);
        }
    } 
    moveSuccess = false; */
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
        /* if (response.ok){
            const result = await response.text();
            if (result == 'Game is finished'){
                alert('we done');
            }
            if (result.length > 14){
                engineMove = result.substring(result.length - 4, result.length);
            }
            moveSuccess = (result.includes('Move success'));
        } */
       if (response.ok){
        
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
            playingAgainstEngine = true;
        }
    } catch (error){
        console.log(error, 'error info');
    } 
}