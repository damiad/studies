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
  $("#file_tree").on("click", ".file-link", function (event) {
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

if (!localStorage.getItem("relevant")) {
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

document.getElementById("menuContainer").style.display = "none";
function openAddCatalogForm() {
  $.ajax({
    type: "POST",
    url: "/aww/add_catalog_form/",
    dataType: "json",
    success: function (response) {
      if (response.success) {
        var updatedHTML = response.html;
        $("#menuContainer").html(updatedHTML);
        document.getElementById("menuContainer").style.display = "block";
      } else {
        alert("Wystąpił błąd podczas otwierania formularza.");
      }
    },
    error: function (xhr, status, error) {
      alert("Wystąpił błąd otwierania formularza" + error);
    },
  });
}

function submitAddCatalogForm(event) {
  event.preventDefault();
  var form = document.getElementById("addCatalogForm");
  var formData = new FormData(form);

  $.ajax({
    type: "POST",
    url: "/aww/add_catalog/",
    data: formData,
    processData: false,
    contentType: false,
    success: function (response) {
      var updatedHTML = response.html;
      $("#file_tree").html(updatedHTML);
      document.getElementById("menuContainer").style.display = "none";
      // alert("Katalog został dodany.");
    },
    error: function (xhr, status, error) {
      alert("Wystąpił błąd podczas dodawania katalogu: " + error);
    },
  });
}

function closeMenuContainer() {
  document.getElementById("menuContainer").style.display = "none";
}

function openRemoveCatalogForm() {
  $.ajax({
    type: "POST",
    url: "/aww/remove_catalog_form/",
    dataType: "json",
    success: function (response) {
      if (response.success) {
        var updatedHTML = response.html;
        $("#menuContainer").html(updatedHTML);
        document.getElementById("menuContainer").style.display = "block";
      } else {
        alert("Wystąpił błąd podczas otwierania formularza.");
      }
    },
    error: function (xhr, status, error) {
      alert("Wystąpił błąd otwierania formularza" + error);
    },
  });
}

function submitRemoveCatalogForm(event) {
  event.preventDefault();
  var form = document.getElementById("removeCatalogForm");
  var formData = new FormData(form);
  $.ajax({
    type: "POST",
    url: "/aww/delete_catalog/",
    data: formData,
    processData: false,
    contentType: false,
    success: function (response) {
      var updatedHTML = response.html;
      $("#file_tree").html(updatedHTML);
      document.getElementById("menuContainer").style.display = "none";
      // alert("Katalog został usunięty.");
    },
    error: function (xhr, status, error) {
      alert("Wystąpił błąd podczas usuwania katalogu: " + error);
    },
  });
}

function openRemoveFileForm() {
  $.ajax({
    type: "POST",
    url: "/aww/remove_file_form/",
    dataType: "json",
    success: function (response) {
      if (response.success) {
        var updatedHTML = response.html;
        $("#menuContainer").html(updatedHTML);
        document.getElementById("menuContainer").style.display = "block";
      } else {
        alert("Wystąpił błąd podczas otwierania formularza.");
      }
    },
    error: function (xhr, status, error) {
      alert("Wystąpił błąd otwierania formularza" + error);
    },
  });
}

function submitRemoveFileForm(event) {
  event.preventDefault();
  var form = document.getElementById("removeFileForm");
  var formData = new FormData(form);
  $.ajax({
    type: "POST",
    url: "/aww/delete_file/",
    data: formData,
    processData: false,
    contentType: false,
    success: function (response) {
      var updatedHTML = response.html;
      $("#file_tree").html(updatedHTML);
      document.getElementById("menuContainer").style.display = "none";
      // alert("Plik został usunięty.");
    },
    error: function (xhr, status, error) {
      alert("Wystąpił błąd podczas usuwania pliku: " + error);
    },
  });
}

function openAddFileForm() {
  $.ajax({
    type: "POST",
    url: "/aww/add_file_form/",
    dataType: "json",
    success: function (response) {
      if (response.success) {
        var updatedHTML = response.html;
        $("#menuContainer").html(updatedHTML);
        document.getElementById("menuContainer").style.display = "block";
      } else {
        alert("Wystąpił błąd podczas otwierania formularza.");
      }
    },
    error: function (xhr, status, error) {
      alert("Wystąpił błąd otwierania formularza" + error);
    },
  });
}

function submitAddFileForm(event) {
  event.preventDefault();
  var form = document.getElementById("addFileForm");
  var formData = new FormData(form);

  $.ajax({
    type: "POST",
    url: "/aww/add_file/",
    data: formData,
    processData: false,
    contentType: false,
    success: function (response) {
      var updatedHTML = response.html;
      var fileId = response.file_id;
      localStorage.setItem("fileId", fileId);
      $("#menuContainer").html(updatedHTML);
    },
    error: function (xhr, status, error) {
      alert("Wystąpił błąd podczas dodawania pliku: " + error);
    },
  });
}

document.getElementById("menuContainer").style.maxHeight = "500px";
document.getElementById("menuContainer").style.overflowY = "scroll";
function saveSections(event) {
  event.preventDefault();
  var form = document.getElementById("saveSectionsForm");
  var formData = new FormData(form);
  var sectionsData = [];
  fileId = localStorage.getItem("fileId");
  // Iteruje dane formularza i pobiera wartości poszczególnych pól sekcji
  for (var [key, value] of formData.entries()) {
    if (key.startsWith("start_line_")) {
      var sectionIndex = key.split("_")[2];
      var sectionData = {
        start_line: value,
        end_line: formData.get("end_line_" + sectionIndex),
        section_type: formData.get("section_type_" + sectionIndex),
        name: formData.get("name_" + sectionIndex),
        description: formData.get("description_" + sectionIndex),
        content: formData.get("content_" + sectionIndex),
        file_id: fileId,
      };
      sectionsData.push(sectionData);
    }
  }
  $.ajax({
    type: "POST",
    url: "/aww/save_sections/",
    data: JSON.stringify(sectionsData),
    processData: false,
    contentType: "application/json",
    success: function (response) {
      var updatedHTML = response.html;
      $("#file_tree").html(updatedHTML);
      document.getElementById("menuContainer").style.display = "none";
      // alert("Plik został dodany");
    },
    error: function (xhr, status, error) {
      alert("Wystąpił błąd podczas zapisywania sekcji: " + error);
    },
  });
}

var asm_code = localStorage.getItem("asm_code");
$("#show-all-sections").hide();
$("#save-button").hide();
function compile() {
  var fileId = localStorage.getItem("fileId");
  var c_standard = localStorage.getItem("c_standard");
  var procesor = localStorage.getItem("procesor");
  var optymalizacje = localStorage.getItem("optymalizacje");
  var relevant = localStorage.getItem("relevant");
  $("#show-all-sections").hide();
  $("#save-button").hide();
  if (!fileId) {
    alert("Nie wybrano pliku do skompilowania.");
    return;
  }
  $.ajax({
    url:
      "/aww/main/compile_file/" +
      fileId +
      "/" +
      c_standard +
      "/" +
      procesor +
      "/" +
      optymalizacje +
      "/" +
      relevant +
      "/",
    data: {},
    dataType: "json",
    success: function (response) {
      var data = response.text;
      if (response.kompilacja) {
        var sectionTitle = "";
        var sectionContent = "";
        var html = "";
        var isTitle = false;
        var lines = data.split("\n");

        for (var i = 0; i < lines.length; i++) {
          var line = lines[i].trim();

          if (isTitle) {
            sectionTitle += line + "\n";
            if (line.startsWith(";----")) {
              var sectionHTML =
                '<div class="asm-section">' +
                '<div class="asm-section-header">' +
                "<pre>" +
                sectionTitle +
                "</pre>" +
                "</div>";
              html += sectionHTML;
              isTitle = false;
              sectionContent = "";
            }
          } else {
            if (line.startsWith(";----")) {
              isTitle = true;
              sectionTitle = line + "\n";
              var sectionHTML =
                '<div class="asm-section-content">' +
                "<pre>" +
                sectionContent +
                "</pre>" +
                "</div>" +
                "</div>";
              html += sectionHTML;
            } else {
              sectionContent += line + "\n";
              if (i === lines.length - 1) {
                var sectionHTML =
                  '<div class="asm-section-content">' +
                  "<pre>" +
                  sectionContent +
                  "</pre>" +
                  "</div>" +
                  "</div>";
                html += sectionHTML;
              }
            }
          }
        }
        localStorage.setItem("asm_code", data);
        $("#fragment-kodu pre code").html(html);
        $("#show-all-sections").show();
        $("#save-button").show();
      } else {
        var error_lines = data.split("\n");
        var html = "<div><h3> Kompilacja nie powiodła się :( <h3></div>";
        for (var i = 0; i < error_lines.length; i++) {
          var line = error_lines[i].trim();
          var line_number = parseInt(line.split(":")[0]);

          if (line_number) {
            html +=
              '<a href="#" onmousedown="showError(' +
              line_number +
              '); return false;">' +
              line +
              "</a><br>";
          } else {
            html += line + "<br>";
          }
        }
        $("#fragment-kodu pre code").html(html);
      }
    },
    error: function (xhr, status, error) {
      alert("Wystąpił błąd podczas kompilacji: " + error);
    },
  });
}

$(document).ready(function () {
  // Schowanie/pokazywanie treści wszystkich sekcji
  $("#show-all-sections").on("click", function () {
    $(".asm-section-content").toggle();
  });
});
// Schowanie/pokazywanie treści pojedynczej sekcji
$(document).on("click", ".asm-section-header", function () {
  $(this).next(".asm-section-content").toggle();
});

function saveToFile() {
  var code = localStorage.getItem("asm_code");
  if (!code) {
    alert("Nie ma żadnego kodu do zapisania.");
    return;
  }
  $.ajax({
    type: "POST",
    url: "/aww/main/save_asm_file/",
    data: { asm_code: code },
    success: function (data) {
      alert(
        "Plik został zapisany na dysku w głównm folderze django jako file.asm"
      );
    },
    error: function (xhr, status, error) {
      alert("Wystąpił błąd podczas zapisywania pliku: " + error);
    },
  });
}
$("#save-button").click(saveToFile);


var showErrorTimeout;

function showError(lineNumber) {
  var sourceCode = $("#tekst-programu pre code").text().split("\n");
  for (var i = 0; i < sourceCode.length; i++) {
    sourceCode[i] = sourceCode[i].trim();
  }
  var lineText = sourceCode[lineNumber - 1];

  // Wyświetla mini okienko z tekstem wiersza
  var tooltip = $("<div class='tooltip'></div>")
    .text(lineText)
    .css({
      position: "fixed",
      background: "#fff",
      border: "1px solid #ccc",
      padding: "5px",
      borderRadius: "3px",
      boxShadow: "0 0 5px rgba(0, 0, 0, 0.3)",
      zIndex: "9999",
      top: event.clientY + 10,
      left: event.clientX + 10,
    });
  $("body").append(tooltip);

  // Ustala pozycję mini okienka względem kursora
  $(document).on("mousemove", function(event) {
    tooltip.css({
      top: event.clientY + 10,
      left: event.clientX + 10
    });
  });

  // Ukrywa mini okienko po zakończeniu przytrzymania linku
  $(document).on("mouseup", function() {
    clearTimeout(showErrorTimeout);
    tooltip.remove();
    $(document).off("mousemove");
    $(document).off("mouseup");
  });

  // Okienko działa maksymalnie 20 sekund (w razie gdyby użytkownik zapomniał o nim)
  showErrorTimeout = setTimeout(function() {
    tooltip.remove();
    $(document).off("mousemove");
    $(document).off("mouseup");
  }, 20000);
}


// Podświetlanie sekcji i wyróżnianie nagłówka i treści sekcji po najechaniu myszką
$(document).ready(function() {
  $("#fragment-kodu").on("mouseenter", ".asm-section", function() {
    $(this).css({
      "background-color": "yellow"
    });
    var header = $(this).find(".asm-section-header");
    var content = $(this).find(".asm-section-content");
    // Dodanie stylów dla wyróżnienia nagłówka i treści sekcji
    header.css({
      "font-weight": "bold"
    });
    content.css({
      "font-style": "italic"
    });
  });
  // Przypisanie zdarzenia hover do rodzica sekcji
  $("#fragment-kodu").on("mouseleave", ".asm-section", function() {
    // Usunięcie stylów podświetlenia sekcji
    $(this).css({
      "background-color": "transparent"
    });
    var header = $(this).find(".asm-section-header");
    var content = $(this).find(".asm-section-content");
    header.css({
      "font-weight": "normal"
    });
    content.css({
      "font-style": "normal"
    });
  });
});
