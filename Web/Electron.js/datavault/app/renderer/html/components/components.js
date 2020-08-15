function passwordGenerator(item, target, options) {
    item.load("components/pwdgenerator.html");

    var pos = target.offset();
    pos.top += target[0].getBoundingClientRect().height;

    item.css(pos);

    target.on("focus", (e) => {
        item.css("visibility", "visible");
    });

    target.on("focusout", (e) => {
        item.css("visibility", "hidden");
    });

    item.on("click", (e) => {
        console.log("click");
        target.trigger("focus");
    });

    item.find("form input[type=range]").on("change", () => {
        console.log("change");
        item.find("form span.range-result").html($(this).value());
    });
}

let Charsets = {
    printable: "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!#$%&()*+,-./:;<=>?@[]^_{|}~",
    uppercase: "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
    lowercase: "abcdefghijklmnopqrstuvwxyz",
    digits: "0123456789",
    alphanumeric: "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",
    symbols: "!#$%&()*+,-./:;<=>?@[]^_{|}~"
}

module.exports = {
    passwordGenerator,
    Charsets
}