setResponse("events.renderer.page.init", (e, data) => {
    jQuery("#test").text(data["text"]);
});

jQuery("#btn").on("click", () => {
    const res = remote.getGlobal("res");
    if (res) {
        console.log(`sending ${jQuery("#msg").val()}`);
        remote.getGlobal("res").send(jQuery("#msg").val());
    }
});