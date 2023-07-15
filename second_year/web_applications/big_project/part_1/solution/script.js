const themeSwitcher = document.getElementById("theme-switcher");
themeSwitcher.addEventListener("click", function () {
  document.body.classList.toggle("dark");
});

// Get all the tab links and content
const tabLinks = document.querySelectorAll(".tablinks");
const tabContent = document.querySelectorAll(".tabcontent");

// Add a click event listener to each tab link
tabLinks.forEach(function (link) {
  link.addEventListener("click", function (event) {
    event.preventDefault(); // Prevent the link from going to a new page

    // Get the data-tab attribute value of the clicked link
    const tabId = this.getAttribute("data-tab");

    // Hide all the tab content
    tabContent.forEach(function (content) {
      content.style.display = "none";
    });

    // Show the selected tab content
    document.getElementById(tabId).style.display = "block";

    // Remove the 'active' class from all the tab links
    tabLinks.forEach(function (link) {
      link.classList.remove("active");
    });

    // Add the 'active' class to the clicked link
    this.classList.add("active");
  });
});
document.getElementById("tab1").style.display = "block";
