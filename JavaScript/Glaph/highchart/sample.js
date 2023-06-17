Highcharts.ganttChart("container", {
    title: {
        text: "Highcharts Gantt Chart",
    },
    yAxis: {
        uniqueNames: true,
        grid: {
            borderWidth: 0
        },
    },

    navigator: {
        enabled: true,
        liveRedraw: true,
        series: {
            type: 'gantt',
            pointPlacement: 0.5,
            pointPadding: 0.25,
            accessibility: {
                enabled: false
            }
        },

        yAxis: {
            min: 0,
            max: 3,
            reversed: true,
            categories: []
        }
    },
    scrollbar: {
        enabled: true
    },

    rangeSelector: {
        enabled: true,
        selected: 0
    },

    accessibility: {
        point: {
            descriptionFormat: '{yCategory}. ' +
                '{#if completed}Task {(multiply completed.amount 100):.1f}% completed. {/if}' +
                'Start {x:%Y-%m-%d}, end {x2:%Y-%m-%d}.'
        },
        series: {
            descriptionFormat: '{name}'
        }
    },

    legend: {
        enabled: false,
    },
    xAxis: [{
        zoomEnabled: true,
        currentDateIndicator: {
            color: '#2caffe',
            dashStyle: 'ShortDot',
            width: 2,
            label: {
                format: ''
            }
        },
        dateTimeLabelFormats: {
            day: '%e<br><span style="opacity: 0.5; font-size: 0.7em">%a</span>'
        },
        grid: {
            borderWidth: 0
        },
        /*        
        min: today - 3 * day,
                max: today + 18 * day,
        custom: {
            today,
            weekendPlotBands: true
        }
        */
    }],
    plotOptions: {
        series: {
            dataLabels: [{
                enabled: true,
                format: '',
            }, {
                enabled: true,
                format: '',
            }]
        }
    },

    series: [
        {
            name: "",
            data: [{
                x: Date.UTC(2017, 0, 1),
                x2: Date.UTC(2017, 0, 3),
                name: "Test",
                y: 0,
                color: "red"
            }, {
                x: Date.UTC(2017, 0, 4),
                x2: Date.UTC(2017, 0, 5),
                y: 0,
                name: "Test",
                color: "green"

            }, {
                x: Date.UTC(2017, 0, 5),
                x2: Date.UTC(2017, 0, 8),
                y: 0,
                name: "Test",
                color: "pink"
            }
            ]

        },
    ]

},
);

// Activate the custom button
document.getElementById("pdf").addEventListener("click", function () {
    Highcharts.charts[0].exportChart({
        type: "application/pdf",
    });
});
