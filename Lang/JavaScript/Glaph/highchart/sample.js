plotData = [
    {
        pointWidth: undefined,
        name: "OOXX",
        data: [{
            start: Date.UTC(2017, 0, 6, 14),
            end: Date.UTC(2017, 0, 8, 23),
            color: "red",
            y: 4,
            z: 1,
            status: "running"
        }, {
            start: Date.UTC(2017, 0, 8, 12),
            end: Date.UTC(2017, 0, 8, 23),
            color: "green",
            z: 1,
            status: "running"

        }, {
            start: Date.UTC(2017, 0, 8, 12),
            end: Date.UTC(2017, 0, 8, 23),
            color: "pink",
            z: 1,
            status: "hoge"
        }, {
            start: Date.UTC(2017, 0, 8, 12),
            end: Date.UTC(2017, 0, 8, 23),
            y: 1,
            color: "blue",
            status: "fuga"
        },
        {
            start: Date.UTC(2017, 0, 8, 12),
            end: Date.UTC(2017, 0, 8, 23),
            color: "blue",
            status: "hogefuga"
        },
        {
            start: Date.UTC(2017, 0, 8, 12),
            end: Date.UTC(2017, 0, 8, 23),
            color: "yellow",
            status: "hogefuga"
        },
        {
            start: Date.UTC(2017, 0, 8, 12),
            end: Date.UTC(2017, 0, 8, 23),
            color: "yellow",
            status: "hogefuga"
        },
        {
            start: Date.UTC(2017, 0, 8, 12),
            end: Date.UTC(2017, 0, 8, 23),
            color: "yellow",
            status: "hogefuga"
        },
        {
            start: Date.UTC(2017, 0, 8, 12),
            end: Date.UTC(2017, 0, 8, 23),
            color: "yellow",
            status: "hogefuga"
        },
        {
            start: Date.UTC(2017, 0, 8, 12),
            end: Date.UTC(2017, 0, 8, 23),
            color: "yellow"
            , y: 5
            , status: "hogefuga"
        },
        ]
    }]

temp = Highcharts.ganttChart("container", {
    lang: {
        noData: 'NO DATA',
        loading: 'Loading...'
    },
    noData: {
        style: {
            fontWeight: 'bold',
            fontSize: '15px',
            color: 'black'
        }
    },
    title: {
        text: ""// "Highcharts Gantt Chart",
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
    exporting: {
        csv: {
            columnHeaderFormatter: function (item, key) {
                if (!key) {
                    return 'custom title'
                }
                return false
            }
        }
    },
    rangeSelector: {
        enabled: true,
        allButtonsEnabled: true,
        selected: 1,
        inputDateFormat: '%e %b %Y %H:%M',
        inputEditDateFormat: '%e %b %Y %H:%M',
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
        }, /*{
            type: 'ytd',
            text: 'YTD',
            title: 'View year to date'
        }, */{
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
            day: '%e<br><span style="opacity: 0.5; font-size: 0.7em">%a %H:%M</span>'
        },
        grid: {
            borderWidth: 0
        }, gridLineWidth: 1,
        minRange: 60 * 1000, // one hour
        // 特定の日付に線を入れる
        minTickInterval: 1,
        tickInterval: 24 * 3600 * 1000 / 24,
        //tickPixelInterval: 60,
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
            '{point.start:%e. %b  %H:%M} - {point.end:%e. %b  %H:%M}'
        ,
        headerFormat: '<span style="color:{point.color}">●</span>{series.name}'
        , pointFormat: '<b>{point.custom.state}</b>'
        , footerFormat: '{point.start:%e. %b} - {point.end:%e. %b}'
    },
    series: plotData,

},
);

// Activate the custom button
document.getElementById("pdf").addEventListener("click", function () {
    Highcharts.charts[0].exportChart({
        type: "application/pdf",
    });
});
