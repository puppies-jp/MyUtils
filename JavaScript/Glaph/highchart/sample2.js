var ctx = document.getElementById("ctx1");
var scatterChart = new Chart(ctx, {
    type: 'line',
    data: {
        datasets: [
            {

                label: 'Scatter Dataset',
                backgroundColor: "rgba(246,156,85,1)",
                borderColor: "rgba(246,156,85,1)",
                fill: false,
                borderWidth: 15,
                pointRadius: 0,
                data: [
                    {
                        x: 0,
                        y: 9
                    }, {
                        x: 3,
                        y: 9
                    }
                ]
            },
            {
                backgroundColor: "rgba(208,255,154,1)",
                borderColor: "rgba(208,255,154,1)",
                fill: false,
                borderWidth: 15,
                pointRadius: 0,
                data: [
                    {
                        x: 3,
                        y: 7
                    }, {
                        x: 5,
                        y: 7
                    }
                ]
            },
            {

                label: 'Scatter Dataset',
                backgroundColor: "rgba(246,156,85,1)",
                borderColor: "rgba(246,156,85,1)",
                fill: false,
                borderWidth: 15,
                pointRadius: 0,
                data: [
                    {
                        x: 5,
                        y: 5
                    }, {
                        x: 10,
                        y: 5
                    }
                ]
            },
            {
                backgroundColor: "rgba(208,255,154,1)",
                borderColor: "rgba(208,255,154,1)",
                fill: false,
                borderWidth: 15,
                pointRadius: 0,
                data: [
                    {
                        x: 10,
                        y: 3
                    }, {
                        x: 13,
                        y: 3
                    }
                ]
            }
        ]
    },
    options: {
        responsive: true,

        legend: {
            display: false
        },
        scales: {
            xAxes: [{
                type: 'linear',
                position: 'bottom',
                ticks: {
                    beginAtzero: true,
                    stepSize: 1
                }
            }],
            yAxes: [{
                scaleLabel: {
                    display: false
                },
                ticks: {
                    beginAtZero: true,
                    max: 10
                }
            }]
        },
    },
    plugins: {
        zoom: {
            zoom: {
                wheel: {
                    enabled: true
                },
                pinch: {
                    enabled: true
                },
                mode: "xy"
            },
            pan: {
                enabled: true
            }
        }
    }

}
);