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
    defaultLength: 10
});

jQuery("#create-submit").on("click", () => {
    console.log(jQuery("#create-pwd").val());
});