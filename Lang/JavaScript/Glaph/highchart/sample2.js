var ctx = document.getElementById("ctx1");
var nbPoints = 1000;
var samplesPoints = 100;
var thresholdsPoints = 900;
var dataArr = []
for (let i = 0; i < nbPoints; i++) {
    dataArr.push({ x: i, y: Math.floor(Math.random() * 100) })
}

var myChart = new Chart(ctx, {
    type: 'line',
    data: {
        datasets: [{
            label: '# of Votes',
            data: dataArr
        }]
    },
    options: {
        parsing: true,
        normalized: true,
        animation: true,
        responsive: true,
        plugins: {
            title: {
                display: true,
                position: 'bottom',
                text: (ctx) => 'this is title'
            },
            subtitle: {
                display: true,
                position: 'bottom',
                text: "Zoom: 'Shift' + 'Drag&Drop' Pan: 'Drag&Drop'"
            },
            zoom: {
                limits: {
                    x: { min: 0, max: nbPoints }
                },
                pan: {
                    enabled: true,
                    mode: 'xy'
                },
                zoom: {

                    drag: {
                        enabled: true,
                        modifierKey: 'shift',
                    },/* wheel: {
                        enabled: true
                    },*/
                    mode: 'xy'
                }
            }
        },
        scales: {
            x: {
                type: 'linear'
            }
        }
    }
});

function resetZoom() {
    myChart.resetZoom();
}