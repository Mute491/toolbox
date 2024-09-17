<script>

  
    dragElement(document.getElementById("header"));
 
    function dragElement(elmnt) {
        var pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0;
        elmnt.onmousedown = dragMouseDown;
 
        function dragMouseDown(e) {
            e = e || window.event;
            e.preventDefault();
            pos3 = e.clientX;
            pos4 = e.clientY;
            document.onmouseup = closeDragElement;
            document.onmousemove = elementDrag;
        }
 
        function elementDrag(e) {
            e = e || window.event;
            e.preventDefault();
            pos1 = pos3 - e.clientX;
            pos2 = pos4 - e.clientY;
            pos3 = e.clientX;
            pos4 = e.clientY;
 
            // Calcola le nuove posizioni
            var newTop = elmnt.parentElement.offsetTop - pos2;
            var newLeft = elmnt.parentElement.offsetLeft - pos1;
 
            // Limita le nuove posizioni ai confini della finestra
            if (newTop < 0) newTop = 0;
            if (newLeft < 0) newLeft = 0;
            if (newTop + elmnt.parentElement.offsetHeight > window.innerHeight) newTop = window.innerHeight - elmnt.parentElement.offsetHeight;
            if (newLeft + elmnt.parentElement.offsetWidth > window.innerWidth) newLeft = window.innerWidth - elmnt.parentElement.offsetWidth;
 
            elmnt.parentElement.style.top = newTop + "px";
            elmnt.parentElement.style.left = newLeft + "px";
        }
 
        function closeDragElement() {
            document.onmouseup = null;
            document.onmousemove = null;
        }
    }
</script>