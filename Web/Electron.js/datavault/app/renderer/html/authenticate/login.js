jQuery("#file-select").change(() => {
    if (jQuery("#file-select").val() == "new") {
        jQuery("#create").css("display", "inherit");
    }
    else {
        jQuery("#create").css("display", "none");
    }
});

// read possible files

// initialize password generator
components.passwordGenerator("#pwd", "#create-pwd", {
    target: "create-pwd",
    canChangeLength: true,
    canChangeReqs: false,
    defaultLength: 12,
    minLength: 10
});

jQuery("#create-submit").on("click", () => {
    var errorsExist = false;
    components.removeMessage(`[name="no-name"]`);
    components.removeMessage(`[name="no-pwd"]`);
    components.removeMessage(`[name="pwd-mismatch"]`);

    // check to see if name exists
    let name = jQuery("#create-name").val();
    if (name == "") {
        components.addMessage(`[name="no-name"]`, "Please input a name", "danger");
        errorsExist = true;
    }

    // check to see if password is empty
    let pwd = jQuery("#create-pwd").val();
    if (pwd == "") {
        components.addMessage(`[name="no-pwd"]`, "Please input a password", "danger");
        errorsExist = true;
    }

    // check to see if two passwords match
    if (pwd != jQuery("#create-pwd-repeat").val()) {
        components.addMessage(`[name="pwd-mismatch"]`, "The two passwords do not match", "danger");
        errorsExist = true;
    }

    // generate new sheet
    if (!errorsExist) {
        sendData("sheet:create", {
            "name": name,
            "pwd": pwd
        });
    }
});