console.log("hello world");
const fs = require("fs");
const http = require("http");
const process = require("process");
const server = http.createServer((req, res) => {
  if (req.url == '/') {
    fs.readFile('./index.html', (err, html) => { //TODO:每次req都讀一次(IO效率差)，應改為const，只載入一次
      if (err) {
        res.statusCode = 404;
        console.log("open index.html failed");
      } else {
        res.statusCode = 200;
        res.write(html);
      }
      res.end();
    })
  }
  else if (req.url == '/input') {
    req.on('data', (data) => {
      console.log('req data: ' + data.toString());
      if (data.toString() === "close") {
        console.log('server closed');
        res.end();
        process.exit(0);
      }
    });
    req.on('end', () => {
      res.statusCode = 200;
      res.end();
    });
  } else {
    fs.readFile('.' + req.url, (err, html) => { //TODO:每次req都讀一次(IO效率差)，應改為const，只載入一次
      if (err) {
        res.statusCode = 404;
        res.end('not found');
      } else {
        res.statusCode = 200;
        res.write(html);
      }
      res.end();
    })
  }
});


fs.readFile("./test.txt", "utf-8", function (err, data) {
  if (err) {
    console.log(err);
  } else {
    console.log(data.toString());
  }
});

server.listen(80, '0.0.0.0', () => {
  console.log('running at http://0.0.0.0/')
})
