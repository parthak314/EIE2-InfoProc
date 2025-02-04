
from pprint import pprint
import boto3
from boto3.dynamodb.conditions import Key, Attr


def scan_movies(display_movies, dynamodb=None):
    if not dynamodb:
        dynamodb = boto3.resource('dynamodb', region_name='us-east-1')

    table = dynamodb.Table('Movies')

    #scan and get the first page of results
    response = table.scan(FilterExpression=Attr('actors').contains("Tom Hanks"));
    data = response['Items']
    display_movies(data)

    #continue while there are more pages of results
    while 'LastEvaluatedKey' in response:
        response = table.scan(FilterExpression=Attr('actors').contains("Tom Hanks"), ExclusiveStartKey=response['LastEvaluatedKey'])
        data.extend(response['Items'])
        display_movies(data)

    return data

if __name__ == '__main__':
    def print_movies(movies):
        for movie in movies:
            # print(f"\n{movie['year']} : {movie['title']}")
            # pprint(movie['info'])
            print(f"{movie['year']} : {movie['title']}")

    pprint(scan_movies(print_movies))
