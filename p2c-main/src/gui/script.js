const send = document.getElementById('send_btn');
function proc() {
    var xhr = new XMLHttpRequest();
    xhr.open('post', window.location.href.toString() + 'input');
    xhr.send("close");
}


// TODO: 遇到特定KEY,有不同版面配置. ex. 'COPY' -> 'SRC' -> 'DEST'
//                                      'ENV'  -> 'KEY' = 'VALUE'
// TODO: 平面化設計+動畫
// TODO: read json and parser json for output.
//       create object for store json data.

const title = document.getElementById('title');
const plusicon = new_plusicon();
plusicon.addEventListener("click", () => {
    genListBox("", []);
});
title.appendChild(plusicon);

//針對要在變數中檢索的 JSON 檔案，將其網址儲存起來
var requestURL ="http://127.0.0.1:5500/p2c-main/src/gui/"
//把資料轉成json格式
//json資料(陣列或是字串)轉換成gui的物件
//gui的物件轉換成json資料(陣列或是字串)(json.paser)
const listbox_group = document.getElementById("listbox_group");

// Read JSON data (replace with your actual JSON data)讀json資料，即將資料轉換為JSON格式
const jsonData = '{"Command": ["--gui", "--website"], "From": ["debian"], "COPY":[] , "ENV":[]}';



function genListBox(name, context) {
    const listgp = document.createElement("div");
    const titleview = document.createElement("div");
    titleview.className = "titleview";
    const title = document.createElement("h3");
    title.textContent = name;
    titleview.appendChild(title);

    // Check if the name exists in the JSON data我感覺不需要確認他是不是JSON格式，但刪掉之後還是不行
    if (jsonData.hasOwnProperty(name)) {
        // Customize layout based on the key
        const keyData = jsonData[name];
        const inputField = document.getElementById('input_field');


        //他沒有辦法讀到copy是不是因為沒有按enter?下面這個註解是有enter鍵的，可是如果用keydown變數就好像就不能用click
    
        /*inputField.addEventListener('keydown', (event) => {
            if (event.key === 'Enter') {
                const inputValue = inputField.value.trim();
                if (inputValue === 'COPY') {
                    // Create input fields for 'SRC' and 'DEST'
                    const srcInput = document.createElement("input");
                    srcInput.setAttribute("type", "text");
                    srcInput.setAttribute("placeholder", "SRC");
                    titleview.appendChild(srcInput);
        
                    const destInput = document.createElement("input");
                    destInput.setAttribute("type", "text");
                    destInput.setAttribute("placeholder", "DEST");
                    titleview.appendChild(destInput);
        
                    // Clear the input field
                    inputField.value = '';
                }
            }
        });*/

        
        if (name === 'COPY') {
            // For 'COPY', create input fields for 'SRC' and 'DEST'
            const srcInput = document.createElement("div");//原本是input
            srcInput.setAttribute("type", "text");
            srcInput.setAttribute("placeholder", "SRC");
            listbox.appendChild(srcInput);//原本是titleview

            const destInput = document.createElement("div");//原本是input
            destInput.setAttribute("type", "text");
            destInput.setAttribute("placeholder", "DEST");
            listbox.appendChild(destInput);//原本是titleview
        } else if (name === 'ENV') {
            // For 'ENV', create input fields for 'KEY' and 'VALUE' with an equals sign
            const keyInput = document.createElement("div");//原本是input
            keyInput.setAttribute("type", "text");
            keyInput.setAttribute("placeholder", "Key");
            listbox.appendChild(keyInput);

            const equalsSign = document.createElement("span");
            equalsSign.textContent = " = ";
            listbox.appendChild(equalsSign);//原本是titleview

            const valueInput = document.createElement("div");//原本是input
            valueInput.setAttribute("type", "text");
            valueInput.setAttribute("placeholder", "Value");
            listbox.appendChild(valueInput);//原本是titleview
        }
    }
//click跟enter好像不能同食用?
    const plusicon = new_plusicon();
    plusicon.addEventListener("click", () => {
        listbox.appendChild(new_viewbox());
    });
    titleview.appendChild(plusicon);

    const trashicon = new_trashicon();
    trashicon.addEventListener("click", () => {
        listgp.remove();
    });
    titleview.appendChild(trashicon);

    listgp.appendChild(titleview);

    const listbox = document.createElement("div");
    listbox.className = "listbox";
    context.forEach(element => {
        listbox.appendChild(new_viewbox(element));
    });
    listgp.appendChild(listbox);
    listbox_group.appendChild(listgp);
}

function new_viewbox(context) {
    const viewbox = document.createElement("div");
    const option = document.createElement("a");
    option.textContent = context;
    option.addEventListener("dblclick", () => {
        option.style = "display: none;";
        const textinput = document.createElement("input");
        textinput.setAttribute("type", "text");
        textinput.setAttribute("value", option.innerText);
        textinput.addEventListener("focusout", () => {
            option.innerText = textinput.value;
            option.removeAttribute("style");
            textinput.remove();
        });
        option.insertAdjacentElement('afterend', textinput);
        textinput.focus();
    });
    viewbox.appendChild(option);
    const trashicon = new_trashicon();
    trashicon.addEventListener("click", () => {
        viewbox.remove();
    });
    viewbox.appendChild(trashicon);
    return viewbox;
}


function genListBox(name, context) {
    const listgp = document.createElement("div");
    const titleview = document.createElement("div");
    titleview.className = "titleview";
    const title = document.createElement("h3");
    title.textContent = name;
    titleview.appendChild(title);
    const editicon = document.createElementNS("http://www.w3.org/2000/svg", 'svg');
    editicon.setAttribute("height", "1em");
    editicon.setAttribute("viewBox", "0 0 512 512");
    const editpath = document.createElementNS("http://www.w3.org/2000/svg", 'path');
    editpath.setAttribute("d", "M362.7 19.3L314.3 67.7 444.3 197.7l48.4-48.4c25-25 25-65.5 0-90.5L453.3 19.3c-25-25-65.5-25-90.5 0zm-71 71L58.6 323.5c-10.4 10.4-18 23.3-22.2 37.4L1 481.2C-1.5 489.7 .8 498.8 7 505s15.3 8.5 23.7 6.1l120.3-35.4c14.1-4.2 27-11.8 37.4-22.2L421.7 220.3 291.7 90.3z");
    editicon.appendChild(editpath);
    editicon.addEventListener("click", () => {
        title.style = "display: none;";
        const textinput = document.createElement("input");
        textinput.setAttribute("type", "text");
        textinput.setAttribute("value", title.innerText);
        textinput.addEventListener("focusout", () => {
            title.innerText = textinput.value;
            title.removeAttribute("style");
            textinput.remove();
        });
        title.insertAdjacentElement('afterend', textinput);
        textinput.focus();
    });
    titleview.appendChild(editicon);
    const plusicon = new_plusicon();
    plusicon.addEventListener("click", () => {
        listbox.appendChild(new_viewbox());
    });
    titleview.appendChild(plusicon);
    const trashicon = new_trashicon();
    trashicon.addEventListener("click", () => {
        listgp.remove();
    });
    titleview.appendChild(trashicon);
    listgp.appendChild(titleview);
    const listbox = document.createElement("div");
    listbox.className = "listbox";
    context.forEach(element => {
        listbox.appendChild(new_viewbox(element));
    });
    listgp.appendChild(listbox);
    listbox_group.appendChild(listgp);
}

function new_viewbox(context) {
    const viewbox = document.createElement("div");
    const option = document.createElement("a");
    option.textContent = context;
    option.addEventListener("dblclick", () => {
        option.style = "display: none;";
        const textinput = document.createElement("input");
        textinput.setAttribute("type", "text");
        textinput.setAttribute("value", option.innerText);
        textinput.addEventListener("focusout", () => {
            option.innerText = textinput.value;
            option.removeAttribute("style");
            textinput.remove();
        });
        option.insertAdjacentElement('afterend', textinput);
        textinput.focus();
    });
    viewbox.appendChild(option);
    const trashicon = new_trashicon();
    trashicon.addEventListener("click", () => {
        viewbox.remove();
    });
    viewbox.appendChild(trashicon);
    return viewbox;
}

function new_trashicon() {
    const trashicon = document.createElementNS("http://www.w3.org/2000/svg", 'svg');
    trashicon.setAttribute("height", "1em");
    trashicon.setAttribute("viewBox", "0 0 448 512");
    const path = document.createElementNS("http://www.w3.org/2000/svg", 'path');
    path.setAttribute("d", "M135.2 17.7L128 32H32C14.3 32 0 46.3 0 64S14.3 96 32 96H416c17.7 0 32-14.3 32-32s-14.3-32-32-32H320l-7.2-14.3C307.4 6.8 296.3 0 284.2 0H163.8c-12.1 0-23.2 6.8-28.6 17.7zM416 128H32L53.2 467c1.6 25.3 22.6 45 47.9 45H346.9c25.3 0 46.3-19.7 47.9-45L416 128z");
    trashicon.appendChild(path);
    return trashicon;
}

function new_plusicon() {
    const plusicon = document.createElementNS("http://www.w3.org/2000/svg", 'svg');
    plusicon.setAttribute("height", "1em");
    plusicon.setAttribute("viewBox", "0 0 448 512");
    const pluspath = document.createElementNS("http://www.w3.org/2000/svg", 'path');
    pluspath.setAttribute("d", "M256 80c0-17.7-14.3-32-32-32s-32 14.3-32 32V224H48c-17.7 0-32 14.3-32 32s14.3 32 32 32H192V432c0 17.7 14.3 32 32 32s32-14.3 32-32V288H400c17.7 0 32-14.3 32-32s-14.3-32-32-32H256V80z");
    plusicon.appendChild(pluspath);
    return plusicon;
}
// 物件轉json
const jsonDataObject = JSON.parse(jsonData);
genListBox("Command", ["--gui", "--website"]);
genListBox("From", ["debian"]);

genListBox("COPY", ["", ""]);
genListBox("ENV", ["="]);
