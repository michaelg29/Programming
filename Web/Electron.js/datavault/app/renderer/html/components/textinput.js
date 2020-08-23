function addMessage(msgElement, msg, type) {
    jQuery(msgElement).addClass(`msg txt-${type}`);
    jQuery(msgElement).html(msg);
}

function removeMessage(msgElement) {
    jQuery(msgElement).html("");
}

// exports
module.exports = {
    addMessage,
    removeMessage
};