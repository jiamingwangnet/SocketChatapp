class Message {
    constructor(message, name)
    {
        this.message = message;
        this.html = document.createElement("div");
        this.html.className = "msg";

        const msgElem = document.createElement("p");
        msgElem.innerText = message;
        
        const nameElem = document.createElement("p");
        nameElem.innerText = name;
        nameElem.className = "name";

        this.html.appendChild(nameElem);
        this.html.appendChild(msgElem);
    }
}

const SEND_API_URL =  "/api/sendmessage";
const GET_API_URL =   "/api/getmessage";
const textarea = document.querySelector("#message");
const msgDisplay = document.querySelector("#messages");
const nameElem = document.querySelector("#name");

function send()
{
    if(nameElem.value == "" || textarea.value == "" || nameElem.value == " " || textarea.value == " ") return;

    const xhr = new XMLHttpRequest();
    const data = {
        msg: nameElem.value + ':' + textarea.value
    };

    xhr.addEventListener("readystatechange", e => {
        if(xhr.readyState == 4 && xhr.status == 200)
        {
            msgDisplay.scrollTo(0, msgDisplay.scrollHeight);
        }
    });

    xhr.open("POST", SEND_API_URL);

    xhr.send(JSON.stringify(data));
    // xhr.setRequestHeader("Content-Type", "application/json");

    textarea.value = "";
}

function fetch()
{
    const xhr = new XMLHttpRequest();

    xhr.addEventListener("readystatechange", e => {
        if(xhr.readyState == 4 && xhr.status == 200)
        {
            const data = JSON.parse(xhr.responseText);
            msgDisplay.innerHTML = "";
            
            for(const msgstr of data.messages)
            {
                const formatted = {
                    name: msgstr.substring(0, msgstr.indexOf(':')),
                    msg: msgstr.substring(msgstr.indexOf(':') + 1, msgstr.length)
                }

                const msg = new Message(formatted.msg, formatted.name);
                msgDisplay.appendChild(msg.html);
            }

            msgDisplay.scrollTo(0, msgDisplay.scrollHeight);
        }
    });

    xhr.open("POST", GET_API_URL);
    xhr.send();
}

const autorefresh = document.querySelector("#autorefresh");
setInterval(() => {
    if(autorefresh.checked)
    {
        fetch();
    }
}, 500);