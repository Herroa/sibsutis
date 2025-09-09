const imageList = [
  '../res/img/panamera_1.jpg',
  '../res/img/panamera_2.jpg',
  '../res/img/panamera_3.jpg',
  '../res/img/panamera_4.jpg',
  '../res/img/panamera_5.jpg',
  '../res/img/panamera_6.jpg',
  '../res/img/panamera_7.jpg',
  '../res/img/panamera_8.jpg',
  '../res/img/panamera_9.jpg',
  '../res/img/panamera_10.jpg',
  '../res/img/panamera_11.jpg',
  '../res/img/panamera_12.jpg',
  '../res/img/panamera_13.jpg',
  '../res/img/panamera_14.jpg',
  '../res/img/panamera_15.jpg',
  '../res/img/panamera_16.jpg',
  '../res/img/panamera_17.jpg',
  '../res/img/panamera_18.jpg',
  '../res/img/panamera_19.jpg',
  '../res/img/panamera_20.jpg',
  '../res/img/panamera_21.jpg',
  '../res/img/panamera_22.jpg',
  '../res/img/panamera_23.jpg',
  '../res/img/panamera_24.jpg',
  '../res/img/panamera_25.jpg',

]
const img = document.querySelector('.img')
const next = document.getElementById('next')
const previous = document.getElementById('previous')
var numImage = 0
img.src = imageList[numImage]

next.addEventListener('click', () => {
  numImage++
  if (numImage >= imageList.length) {
    numImage = 0
  }
  console.log(numImage >= imageList.length)
  console.log(numImage)
  img.src = imageList[numImage]
})
previous.addEventListener('click', () => {
  numImage--
  if (numImage < 0) {
    numImage = imageList.length - 1
  }
  console.log(numImage >= imageList.length)
  console.log(numImage)
  img.src = imageList[numImage]
})