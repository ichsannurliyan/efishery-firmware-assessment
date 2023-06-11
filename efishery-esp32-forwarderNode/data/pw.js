  function activeSide(x) {
    var statusNavigation = document.getElementById("statNav");
    var interfaceNavigation = document.getElementById("intNav");
    var settingNavigation = document.getElementById("setNav");
    var contactNavigation = document.getElementById("conNav");
    if (x == 0) {
      statusNavigation.className = "active"
      interfaceNavigation.className = ""
      settingNavigation.className = ""
      contactNavigation.className = ""
    } else if (x == 1) {
      interfaceNavigation.className = "active"
      statusNavigation.className = ""
      settingNavigation.className = ""
      contactNavigation.className = ""
    } else if (x == 2) {
      settingNavigation.className = "active"
      interfaceNavigation.className = ""
      statusNavigation.className = ""
      contactNavigation.className = ""
    } else if (x == 3) {
      contactNavigation.className = "active"
      interfaceNavigation.className = ""
      statusNavigation.className = ""
      settingNavigation.className = ""
    }
  }

  /* Loop through all dropdown buttons to toggle between hiding and showing its dropdown content - This allows the user to have multiple dropdowns without any conflict */
  var dropdown = document.getElementsByClassName("dropdown-btn");
    var i;

  for (i = 0; i < dropdown.length; i++) {
      dropdown[i].addEventListener("click", function() {
          this.classList.toggle("active");
          var dropdownContent = this.nextElementSibling;
          var caret = document.getElementsByClassName("dropdown-btn").children;
          
          if (dropdownContent.style.display === "block") {
              dropdownContent.style.display = "none";
              caret.style.backgroundColor = "yellow";
              alert(caret.value);
              caret.classList.remove("fa fa-caret-down"); 
          } else {
              dropdownContent.style.display = "block";
          }
      });
  }
  // function loadDoc() {
  //   var xhttp = new XMLHttpRequest();
  //   xhttp.onreadystatechange = function() {
  //     if (this.readyState == 4 && this.status == 200) {
  //     document.getElementById("status").innerHTML = this.responseText;
  //     }
  //   };
  //   alert(this.status)
  //   xhttp.open("GET", "ajax_info.txt", true);
  //   xhttp.send();
  // }