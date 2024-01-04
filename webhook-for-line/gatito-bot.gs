var CHANNEL_ACCESS_TOKEN = 'K60vgYr6Ks9ut4badfSLLkSAPqJgykcPPEQldA/E+I5o354aWooJnyu1QKc5k5cfAhnsbwyLl7LNg/XrNrJgOojmpODClsI69wTK81hq3QoXzS76BtdJU2X1GWuzThcQRMjwDa8KPJjmwXXxmcxeXgdB04t89/1O/w1cDnyilFU='; 
var sheet_url = "https://docs.google.com/spreadsheets/d/1ZCuj0-3AKcwWawn5OAm4ufcgzkuO6D-n8VyA_kbY0GU/edit";

var sheet_name = "Sheet1";
var line_endpoint = 'https://api.line.me/v2/bot/message/reply';


function doPost(e) {
  var json = JSON.parse(e.postData.contents);

  
  var reply_token= json.events[0].replyToken;
  if (typeof reply_token === 'undefined') {
    return;
  }

 
  var message = json.events[0].message.text;
  var userId = JSON.parse(e.postData.contents).events[0].source.userId;
  var username = getUsername(userId);

  var reply_txt = GetReply(message);

  var button_txt = message == 'Energy' ? 'Feed The Cat' : (message == 'Loveliness' ? 'Pet the Cat' : (message == 'Happiness' ?'Play With Your Cat' : ' '));
  var command_txt = message == 'Energy' ? 'Feed' : (message == 'Loveliness' ? 'Pet' : (message == 'Happiness' ? ' ' : ' '));
  var reply_type = (message == 'Feed' || message == 'Pet') ? 1 : 0;
  message  = message + (message == 'Energy' ? '⚡' : (message == 'Loveliness' ? '💓' : (message == 'Happiness' ? '😁' : ' ')));

  var flex_message_status = {
        'type': 'flex',
        "altText": "this is a flex message",
        'contents': {
          "type": "bubble",
          "body": {
            "type": "box",
            "layout": "vertical",
            "contents": [
              {
                "type": "text",
                "text": "You Pet " + message,
                "weight": "bold",
                "size": "xl",
                "align": "center"
              },
              {
                "type": "box",
                "layout": "vertical",
                "margin": "lg",
                "spacing": "sm",
                "contents": [
                  {
                    "type": "text",
                    "text": reply_txt + "%",
                    "size": "xxl",
                    "weight": "bold",
                    "align": "center"
                  }
                ]
              }
            ]
          },
          "footer": {
            "type": "box",
            "layout": "vertical",
            "spacing": "sm",
            "contents": [
              {
                "type": "button",
                "style": "primary",
                "height": "sm",
                "action": {
                  "type": "message",
                  "text": command_txt,
                  "label": button_txt
                },
                "color": "#bf5f00"
              },
              {
                "type": "box",
                "layout": "vertical",
                "contents": [],
                "margin": "sm"
              }
            ],
            "flex": 0
          }
        },
      }
  var message_status = {
    "type":"text",
    "text": reply_txt
  }

  UrlFetchApp.fetch(line_endpoint, {
    'headers': {
      'Content-Type': 'application/json; charset=UTF-8',
      'Authorization': 'Bearer ' + CHANNEL_ACCESS_TOKEN,
    },
    'method': 'post',
    'payload': JSON.stringify({
      'replyToken': reply_token,
      'messages': [
        reply_type ? message_status : flex_message_status
      ],
    }),
  });
  

  return ContentService.createTextOutput(JSON.stringify({'content': 'post ok'})).setMimeType(ContentService.MimeType.JSON);
}


function GetReply(message){
  var spreadsheet = SpreadsheetApp.openByUrl(sheet_url);
  var sheet = spreadsheet.getSheetByName(sheet_name);
  var lr = sheet.getLastRow();
 
  var message_col=1;
  var reply_col=2;
  var start_row=2; 
  
  var reply_txt="";
  for (var i = start_row; i <= lr+1; i++){
    
    if (i == lr+1){
      var reply_txt="I dont't understand. Please use Rich Menu."
    }
    
    var temp_txt = sheet.getRange(i,message_col).getValue();   
    Logger.log(temp_txt);
    if (message == temp_txt){
      var current_value = sheet.getRange(i, reply_col).getValue();
      if (temp_txt == 'Feed'){
        if(!current_value){
          sheet.getRange(i, reply_col).setValue(1);
          reply_txt = 'Feeding'
        }else{
          reply_txt = 'Already Feeding'
        }
        break
      }else if (temp_txt == 'Pet'){
        if(!current_value){
          sheet.getRange(i, reply_col).setValue(1);
          reply_txt = 'Petting'
        }else{
          reply_txt = 'Already Petting'
        }
        break;
      }else{
        var reply_txt = sheet.getRange(i,reply_col).getValue(); 
        break; 
      }
    }
    
  };
  
  return reply_txt;
}

function getUsername(userId) {
  var url = 'https://api.line.me/v2/bot/profile/' + userId;
  var response = UrlFetchApp.fetch(url, {
    'headers': {
      'Authorization': 'Bearer ' + CHANNEL_ACCESS_TOKEN
    }
  });
  return JSON.parse(response.getContentText()).displayName;
}
