

function getSolarInfo(deviceId){
  var response;
  var req = new XMLHttpRequest();
  req.open('GET', "http://www.enricobassetti.it/aurora/index.json.php?" +
    "deviceid=" + deviceId, true);  
 
  req.onload = function(e) {
    console.log("readyState:" + req.readyState);
    if (req.readyState == 4) {
      console.log("status:" + req.status);
      if(req.status == 200) {        
        console.log("responseText:" +req.responseText);
        response = JSON.parse(req.responseText);
        var currentPower, dailyEnergy, monthlyEnergy, yearlyEnergy, totalEnergy;
        
        if (response) {
          currentPower = response.gridp;
          dailyEnergy = response.daily;
          monthlyEnergy = response.monthly;
          yearlyEnergy = response.yearly;
          totalEnergy = response.total;
          console.log(currentPower);
          console.log(dailyEnergy);
          console.log(monthlyEnergy);
          console.log(yearlyEnergy);
          console.log(totalEnergy);
          
          Pebble.sendAppMessage({
            "1":"Power: " + currentPower,
            "2":"Daily: " + dailyEnergy,
            "3":"Monthly: " + monthlyEnergy,
            "4":"Yearly: " + yearlyEnergy,
            "5":"Total: " + totalEnergy});
        }

      } else {
        console.log("Error");
      }
    }
  };
  req.send(null);
}

var DEVICEID;

Pebble.addEventListener("ready",
                        function(e) {
                          console.log("connect!" + e.ready);                          
                          DEVICEID = localStorage.getItem("DEVICE_ID");                          
                          getSolarInfo(DEVICEID);                          
                          console.log(e.type);
                        });

Pebble.addEventListener("appmessage",
                        function(e) {                          
                          console.log(e.type);                        
                          console.log("payload!" + e.payload.reload);
                          if (e.payload.reload == 1){
                            getSolarInfo(DEVICEID);
                          }                          
                          console.log("message!");
                        });

Pebble.addEventListener("webviewclosed",
                        function(e) {
                          console.log(e.type);
                          console.log(e.response);
                          var options = JSON.parse(decodeURIComponent(e.response));
                          console.log("Options = " + JSON.stringify(options));
                          if (options.deviceid !== ""){
                            DEVICEID = options.deviceid;
                            localStorage.setItem("DEVICE_ID", DEVICEID);
                            getSolarInfo(DEVICEID);
                          }
                          
                        });


Pebble.addEventListener("showConfiguration", function() {
  console.log("showing configuration");
  var random = Math.floor((Math.random() * 100) + 1);
  Pebble.openURL('http://www.webrandsrl.com/pebble/configuration.html?id='+random);
});