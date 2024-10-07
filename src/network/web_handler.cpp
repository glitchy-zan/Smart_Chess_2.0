#include <network/web_handler.h>
#include <network/network.h>

/* displays web page on "/" route */
void handleRoot()
{
    String html = "<html><body>";

    html += "<pre>Game info: <span id='game_info'>" + gameStateToString(currentGameState) + "</span></pre>";
    html += "<pre>Move info: <span id='move_info'>" + moveStateToString(currentMoveState) + "</span></pre>";
    html += "<pre>Piece info: <span id='piece_info'>" + pieceStateToString(currentPieceState) + "</span></pre>";
    html += "<pre>PGN: <span id='pgn_info'>" + gameContext.pgn + "</span></pre>";

    html += "<script>";
    html += "var connection = new WebSocket('ws://' + location.hostname + ':81');";
    html += "connection.onopen = function() { console.log('WebSocket connected'); };";
    html += "connection.onclose = function() { console.log('WebSocket disconnected'); };";
    html += "connection.onerror = function(error) { console.log('WebSocket error: ' + error); };";
    html += "connection.onmessage = function(event) {";
    html += "console.log('WebSocket onMessage');";
    html += "var data = JSON.parse(event.data);";
    html += "document.getElementById('game_info').innerText = data.game;";
    html += "document.getElementById('move_info').innerText = data.move;";
    html += "document.getElementById('piece_info').innerText = data.piece;";
    html += "document.getElementById('pgn_info').innerText = data.pgn;";
    html += "};";
    html += "</script>";

    html += "</body></html>";

    server.send(200, "text/html", html);
}
