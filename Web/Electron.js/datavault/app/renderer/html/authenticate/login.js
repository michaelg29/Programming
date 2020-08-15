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
components.passwordGenerator(jQuery("#pwd"), jQuery("#create-pwd"), {
    target: "create-pwd",
    canChange: true,
    charset: components.Charsets.printable
});