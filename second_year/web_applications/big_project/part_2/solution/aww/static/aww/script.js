const themeSwitcher = document.getElementById("theme-switcher");
var theme = localStorage.getItem("theme");
if (theme == "dark") {
  document.body.classList.toggle("dark");
}
themeSwitcher.addEventListener("click", function () {
  if (document.body.classList.contains("dark")) {
    localStorage.setItem("theme", "light");
  } else {
    localStorage.setItem("theme", "dark");
  }
  document.body.classList.toggle("dark");
});

// Get all the tab links and content
const tabLinks = document.querySelectorAll(".tablinks");
const tabContent = document.querySelectorAll(".tabcontent");
tabLinks.forEach(function (link) {
  link.addEventListener("click", function (event) {
    event.preventDefault();
    const tabId = this.getAttribute("data-tab");
    tabContent.forEach(function (content) {
      content.style.display = "none";
    });
    document.getElementById(tabId).style.display = "block";
    tabLinks.forEach(function (link) {
      link.classList.remove("active");
    });
    this.classList.add("active");
  });
});
document.getElementById("tab1").style.display = "block";

// Funkcja zamieniająca znaki specjalne na encje HTML
function escapeHTML(unsafeString) {
  return unsafeString
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;")
    .replace(/'/g, "&#039;");
}

var fileId = localStorage.getItem("fileId");
$(document).ready(function () {
  $(".file-link").on("click", function (event) {
    event.preventDefault();
    var fileId = $(this).data("file-id");
    localStorage.setItem("fileId", fileId);
    $.ajax({
      url: "/aww/main/get_file_sections/" + fileId + "/",
      dataType: "json",
      success: function (data) {
        var sectionTexts = data.section_texts;
        var textProgramu = $("#tekst-programu pre code");
        textProgramu.empty();
        for (var i = 0; i < sectionTexts.length; i++) {
          var sectionContent = escapeHTML(sectionTexts[i]);
          textProgramu.append("<p>" + sectionContent + "</p>");
        }
      },
    });
  });
});

if (!localStorage.getItem("c_standard")) {
  localStorage.setItem("c_standard", "c11");
}
function saveStandard() {
  var selected = document.getElementById("standard-select").value;
  localStorage.setItem("c_standard", selected);
  alert("Wybrany standard to: " + selected);
}


const options = [
  { name: "-fomit-frame-pointer", value: 1 },
  { name: "-finline-functions", value: 2 },
  { name: "-fno-reorder-blocks", value: 4 },
  { name: "-fno-jump-tables", value: 8 },
  { name: "-fpack-struct", value: 16 },
];
const optionsMap = {};
options.forEach((option) => {
  optionsMap[option.name] = option.value;
});


if (!localStorage.getItem("optymalizacje")) {
  localStorage.setItem("optymalizacje", 0);
}
function saveOptymalizacje() {
  const checkboxes = document.querySelectorAll(
    'input[name="optymalizacje[]"]:checked'
  );
  const selectedOptions = [];
  checkboxes.forEach((checkbox) => {
    selectedOptions.push(checkbox.value);
  });

  alert("Wybrane optymalizacje: " + selectedOptions.join(", "));

    const sum = selectedOptions.reduce((acc, option) => {
      return acc + optionsMap[option];
    }, 0);
    localStorage.setItem("optymalizacje", sum);
}


if (!localStorage.getItem("procesor")) {
  localStorage.setItem("procesor", "mcs51");
}
function saveProcesor() {
  var selected = document.getElementById("procesor-select").value;
  localStorage.setItem("procesor", selected);
  if (selected === "mcs51") {
    document.getElementById("zależne-mcs51").style.display = "block";
    document.getElementById("zależne-z80").style.display = "none";
    document.getElementById("zależne-stm8").style.display = "none";
  } else if (selected === "z80") {
    document.getElementById("zależne-mcs51").style.display = "none";
    document.getElementById("zależne-z80").style.display = "block";
    document.getElementById("zależne-stm8").style.display = "none";
  } else if (selected === "stm8") {
    document.getElementById("zależne-mcs51").style.display = "none";
    document.getElementById("zależne-z80").style.display = "none";
    document.getElementById("zależne-stm8").style.display = "block";
  }
  alert("Wybrany procesor to: " + selected);
}

if(!localStorage.getItem("relevant")) {
  localStorage.setItem("relevant", 0);
}

const procMCS51 = {
  1: "Small",
  2: "Medium",
  3: "Large",
  4: "Huge",
};

function saveMcs51Options() {
  const modelProgramu = document.getElementById("model-programu-select").value;
  localStorage.setItem("relevant", modelProgramu);
  alert(`Wybrano model programu: ${procMCS51[modelProgramu]}`);
}

const procZ80 = {
  1: "Small",
  2: "Medium",
  3: "Large",
};

function saveZ80Options() {
  const modelPamięci = document.getElementById("model-pamięci-select").value;
  localStorage.setItem("relevant", modelPamięci);
  alert(`Wybrano model programu: ${procZ80[modelPamięci]}`);
}

const procSTM80 = {
  1: "--fmem-none",
  2: "--fmem-small",
  3: "--fmem-medium",
};

function saveStm8Options() {
  const konfiguracjaPamięci = document.getElementById(
    "konfiguracja-pamięci-select"
  ).value;
  localStorage.setItem("relevant", konfiguracjaPamięci);
  alert(`Wybrano model programu: ${procSTM80[konfiguracjaPamięci]}`);
}

function compile() {
  var fileId = localStorage.getItem("fileId");
  var c_standard = localStorage.getItem("c_standard");
  var procesor = localStorage.getItem("procesor");
  var optymalizacje = localStorage.getItem("optymalizacje");
  var relevant = localStorage.getItem("relevant");
  if (!fileId) {
    alert("Nie wybrano pliku do skompilowania.");
    return;
  }
  $.ajax({
    url: "/aww/main/compile_file/" + fileId + "/" + c_standard + "/" + procesor + "/" + optymalizacje + "/" + relevant + "/",
    data: {},
    success: function (data) {
      $("#fragment-kodu pre code").text(data);
    },
    error: function (xhr, status, error) {
      alert("Wystąpił błąd podczas kompilacji: " + error);
    },
  });
}

function saveToFile() {
  var code = $("#fragment-kodu pre code").text();
  if (!code) {
    alert("Nie ma żadnego kodu do zapisania.");
    return;
  }
  $.ajax({
    type: "POST",
    url: "/aww/main/save_asm_file/",
    data: { asm_code: code },
    success: function (data) {
      alert("Plik został zapisany na dysku w głównm folderze django jako file.asm");
    },
    error: function (xhr, status, error) {
      alert("Wystąpił błąd podczas zapisywania pliku: " + error);
    },
  });
}
$("#save-button").click(saveToFile);
