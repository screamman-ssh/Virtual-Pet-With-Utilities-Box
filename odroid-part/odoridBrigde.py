import googleSheetAPI, weatherAPI

weather = weatherAPI.get_waether()
sheet_data = googleSheetAPI.read_sheet()

print('Parameter, Value:')
for row in sheet_data:
    print('%s, %s' % (row[0], row[1]))