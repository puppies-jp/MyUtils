Highcharts.ganttChart("container", {
    lang: {
        noData: 'NO DATA'
    },
    noData: {
        style: {
            fontWeight: 'bold',
            fontSize: '15px',
            color: '#303030'
        }
    },
    title: {
        text: "Highcharts Gantt Chart",
    },
    yAxis: {
        uniqueNames: true,
        grid: {
            borderWidth: 0
        },
        min: 0,
        max: 4,
        categories: ["First", "Second", "Third", "Four", "Five", "Six"]
        , scrollbar: {
            enabled: true
        },
    },
    credits: {
        enabled: false
    },
    navigator: {
        enabled: true,
        liveRedraw: true,
        series: {
            type: 'gantt',
            //            pointPlacement: 0.5,
            //            pointPadding: 0.25,
            accessibility: {
                enabled: false
            }
        },
        yAxis: {
            min: 0,
            max: 50,
            reversed: true,
            categories: ["First", "Second", "Third", "Four", "Five", "Six"]
        }
    },
    scrollbar: {
        enabled: true
    },

    rangeSelector: {
        enabled: true,
        selected: 0
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
            borderWidth: 0.5
        },
        tickWidth: 1
    }],
    tooltip: {

        format: '<span style="color:{point.color}">●</span>{series.name}:' +
            '<b>{point.custom.state}</b><br/>' +
            '{point.start:%e. %b} - {point.end:%e. %b}'
        ,
        headerFormat: '<span style="color:{point.color}">●</span>{series.name}'
        , pointFormat: '<b>{point.custom.state}</b>'
        , footerFormat: '{point.start:%e. %b} - {point.end:%e. %b}'
    },
    //series: [{ data: [] }],/*
    series: [
        {
            name: "OOXX",
            data: [{
                start: Date.UTC(2017, 0, 1),
                end: Date.UTC(2017, 0, 3),
                name: "Test",
                color: "red",
                y: 4,
                custom: {
                    state: "normal"
                }
            }, {
                start: Date.UTC(2017, 0, 4),
                end: Date.UTC(2017, 0, 6),
                name: "Test",
                color: "green",
                custom: {
                    state: "running"
                }

            }, {
                start: Date.UTC(2017, 0, 5),
                end: Date.UTC(2017, 0, 8),
                name: "Test",
                color: "pink"
                , custom: {
                    state: "stop"
                }
            }, {
                start: Date.UTC(2017, 0, 8, 12),
                end: Date.UTC(2017, 0, 8, 23),
                y: 1,
                name: "Test1",
                color: "blue"
                , custom: {
                    state: 3
                }
            },
            {
                start: Date.UTC(2017, 0, 8, 12),
                end: Date.UTC(2017, 0, 8, 23),
                name: "Test2",
                color: "blue"
                , custom: {
                    state: 3
                }
            },
            {
                start: Date.UTC(2017, 0, 8, 12),
                end: Date.UTC(2017, 0, 8, 23),
                name: "Test3",
                color: "yellow"
                , custom: {
                    state: 3
                }
            },
            {
                start: Date.UTC(2017, 0, 8, 12),
                end: Date.UTC(2017, 0, 8, 23),
                name: "Test4",
                color: "yellow"
                , custom: {
                    state: 3
                }
            },
            {
                start: Date.UTC(2017, 0, 8, 12),
                end: Date.UTC(2017, 0, 8, 23),
                name: "Test5",
                color: "yellow"
                , custom: {
                    state: 3
                }
            },
            {
                start: Date.UTC(2017, 0, 8, 12),
                end: Date.UTC(2017, 0, 8, 23),
                name: "Test6",
                color: "yellow"
                , custom: {
                    state: 3
                }
            },
            {
                start: Date.UTC(2017, 0, 8, 12),
                end: Date.UTC(2017, 0, 8, 23),
                name: "Test7",
                color: "yellow"
                , y: 5
                , custom: {
                    state: 3
                }
            },
            ]

        },
    ]
    //* /
},
);

// Activate the custom button
document.getElementById("pdf").addEventListener("click", function () {
    Highcharts.charts[0].exportChart({
        type: "application/pdf",
    });
});
