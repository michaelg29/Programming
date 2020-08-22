function passwordGenerator(itemId, targetId, options) {
    let item = jQuery(itemId);
    let target = jQuery(targetId);

    item.load("components/pwdgenerator.html", () => {
        // link click event in target to opening section
        target.on("focus", () => {
            jQuery(`input[type="checkbox"][name="pwd-toggle"]`).attr("checked", true);
        });

        // link close button to closing section
        jQuery(`a.btn-danger[name="close"]`).on("click", () => {
            jQuery(`input[type="checkbox"][name="pwd-toggle"]`).attr("checked", false);
        });

        // link input in target to output in section
        target.on("input", () => {
            jQuery(`input[type="text"][name="output"]`).val(target.val());
        });

        // link input in section to target
        jQuery(`input[type="text"][name="output"]`).on("input", () => {
            target.val(jQuery(`input[type="text"][name="output"]`).val());
        });

        // link range input to its output
        jQuery(`input[type="range"][name="length"]`).on("input", () => {
            item.find("form span.range-result").html(jQuery(`input[type="range"][name="length"]`).val());
        });
        if ("defaultLength" in options) {
            jQuery(`input[type="range"][name="length"]`).val(options.defaultLength);
            item.find("form span.range-result").html(options.defaultLength);
        }

        item.find(`a.btn[name="gen"]`).on("click", () => {
            let names = ["symbols", "lowercase", "uppercase", "digits"];

            let noChars = [];
            let chars = "";

            for (var i = 0; i < names.length; i++) {
                if (jQuery(`input[type="checkbox"][name="chars-${names[i]}"]`).prop("checked")) {
                    // allocate data slot for this name if required
                    noChars.push([names[i], 0]);
                    chars += Charsets[names[i]];
                }
            }

            if (noChars.length == 0) {
                alert("Please check at least 1 box");
                return;
            }

            let charsetSize = chars.length;

            let pwd;
            let pwdLength = jQuery(`input[type="range"][name="length"]`).val();

            // generate random passwords until true
            while (true) {
                // reset variables
                pwd = "";
                for (var i = 0; i < noChars.length; i++) {
                    noChars[i][1] = false;
                }

                // generate password
                for (var i = 0; i < pwdLength; i++) {
                    pwd += chars[Math.floor(Math.random() * charsetSize)];

                    for (var j = 0; j < noChars.length; j++) {
                        if (Charsets[noChars[j][0]].indexOf(pwd[i]) != -1) {
                            // set to true for this category
                            noChars[j][1] = true;
                            break;
                        }
                    }
                }

                // check if meets requirements
                let meetsRequirements = true;
                for (var i = 0; i < noChars.length; i++) {
                    if (!noChars[i][1]) {
                        meetsRequirements = false;
                        break;
                    }
                }

                if (meetsRequirements) {
                    break;
                }
            }

            target.val(pwd);
            jQuery(`input[type="text"][name="output"]`).val(pwd);
        });
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