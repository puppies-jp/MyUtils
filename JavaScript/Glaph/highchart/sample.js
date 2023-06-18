Highcharts.ganttChart("container", {
    lang: {
        noData: 'NO DATA'
    },
    noData: {
        style: {
            fontWeight: 'bold',
            fontSize: '15px',
            color: 'black'
        }
    },
    title: {
        text: "Highcharts Gantt Chart",
    },
    chart: {
        scrollablePlotArea: {
            minHeight: 600,
            opacity: 0,
        }, panning: { enabled: true, type: 'x' },
        zoomType: "x"
    },

    yAxis: {
        uniqueNames: true,
        grid: {
            borderWidth: 0
        },
        //min: 0,
        //max: 4,
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
            //pointPlacement: 0.5,
            //pointPadding: 0.25,
            accessibility: {
                enabled: false
            }
        },
        yAxis: {
            min: 0,
            max: 50,
            reversed: true,
            categories: []
        }
    },
    scrollbar: {
        buttonArrowColor: "black",
        buttonsEnabled: true,
        enabled: true
    },
    rangeSelector: {
        enabled: true,
        selected: 0,
        buttons: [{
            type: 'hour',
            count: 3,
            text: '1h',
            title: 'View 1 day'
        }, {
            type: 'month',
            count: 3,
            text: '3m',
            title: 'View 3 months'
        }, {
            type: 'ytd',
            text: 'YTD',
            title: 'View year to date'
        }, {
            type: 'year',
            count: 1,
            text: '1y',
            title: 'View 1 year'
        }, {
            type: 'all',
            text: 'All',
            title: 'View all'
        }]
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
        panningEnabled: true,
        dateTimeLabelFormats: {
            minute: '%H:%M',
            hour: '%H:%M',
            day: '%e<br><span style="opacity: 0.5; font-size: 0.7em">%a</span>'
        },
        grid: {
            borderWidth: 0
        }, gridLineWidth: 1,

        // 特定の日付に線を入れる
        plotLines: [
            {
                value: Date.UTC(2017, 0, 8, 22), color: "green",
                label: { text: "the day" }, zIndex: 10
            },
            //{ color: "blue", events: { click: PlotLineOrBand } }
        ],
        // ホバー時に線を表示する
        crosshair: { snap: false, color: "black", zIndex: 10 },
    }],
    tooltip: {

        format: '<span style="color:{point.color}">●</span>{series.name}:' +
            '<b>{point.custom.state}{point.status}</b><br/>' +
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
                start: Date.UTC(2017, 0, 8, 14),
                end: Date.UTC(2017, 0, 8, 23),
                name: "Test",
                color: "red",
                y: 4,
                status: "running"
            }, {
                start: Date.UTC(2017, 0, 8, 12),
                end: Date.UTC(2017, 0, 8, 23),
                name: "Test",
                color: "green",
                status: "running"

            }, {
                start: Date.UTC(2017, 0, 8, 12),
                end: Date.UTC(2017, 0, 8, 23),
                name: "Test",
                color: "pink",
                status: "hoge"
            }, {
                start: Date.UTC(2017, 0, 8, 12),
                end: Date.UTC(2017, 0, 8, 23),
                y: 1,
                name: "Test1",
                color: "blue",
                status: "fuga"
            },
            {
                start: Date.UTC(2017, 0, 8, 12),
                end: Date.UTC(2017, 0, 8, 23),
                name: "Test2",
                color: "blue",
                status: "hogefuga"
            },
            {
                start: Date.UTC(2017, 0, 8, 12),
                end: Date.UTC(2017, 0, 8, 23),
                name: "Test3",
                color: "yellow",
                status: "hogefuga"
            },
            {
                start: Date.UTC(2017, 0, 8, 12),
                end: Date.UTC(2017, 0, 8, 23),
                name: "Test4",
                color: "yellow",
                status: "hogefuga"
            },
            {
                start: Date.UTC(2017, 0, 8, 12),
                end: Date.UTC(2017, 0, 8, 23),
                name: "Test5",
                color: "yellow",
                status: "hogefuga"
            },
            {
                start: Date.UTC(2017, 0, 8, 12),
                end: Date.UTC(2017, 0, 8, 23),
                name: "Test6",
                color: "yellow",
                status: "hogefuga"
            },
            {
                start: Date.UTC(2017, 0, 8, 12),
                end: Date.UTC(2017, 0, 8, 23),
                name: "Test7",
                color: "yellow"
                , y: 5
                , status: "hogefuga"
            },
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
