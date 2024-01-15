# Node Types


* Fixed 
* Flyable 
* Movable

# NodeFieldData 

- [Example1](#example)
 
* name
* value 
* category
* categoryIconUrl

## Example1
```JSON
{
        "Name": __name, 
        "Value":{
         __value
        },
        "Category":__category,
        "CategoryIconUrl":__categoryIconUrl
}

```

# NodeData

- [Example2](#example)

* id 
* name 
* type 
* category
* url2D 
* url3D 
* imgInfoUrl
* iconInfoUrl
* color 
* isAttacker
* latitude 
* longitude 
* altidude 
* speed
* command
* layersId 
* fieldData

## Example2

```JSON
{
    "Id" : 100,
    "Name" : "some name",
    "Type" : "some type",
    "Category" : "some category",
    "Url2D": "some url",
    "Url3D": "some other url",
    "ImgInfoUrl": "some image url",
    "IconInfoUrl": "some icon info url",
    "color": "some color",
    "IsAttacker": false,
    "Latitude": 33.33,
    "Longitude": 33.33,
    "Altitude": 33.33,
    "Speed": 100,
    "Command":"Update",
    "LayersId":[
      120,
      121
    ],
    "FieldData":[
        "Name": __name, 
        "Value":{
         __value
        },
        "Category":__category,
        "CategoryIconUrl":__categoryIconUrl
    ]
}
```

# StatusNodeData

- [Example3](#example)

* id
* name
* longitude
* latitude
* altitude
* Command
* LayerId
* fieldData

## Example3

```JSON
{
    "Id" : 100,
    "Name" : "some name",
    "Longitude": 33.3,
    "Latitude": 33.3,
    "Altitude": 33.3,
    "Command": "Update",
    "LayerId": 120,
    "FieldData":[
        "Name": __name, 
        "Value":{
         __value
        },
        "Category":__category,
        "CategoryIconUrl":__categoryIconUrl
    ]
}
```

