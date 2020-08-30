let btn = document.getElementById("changeColor");

chrome.storage.sync.get("color", (data) => {
    btn.style.backgroundColor = data.color;
    btn.setAttribute("value", data.color);
});