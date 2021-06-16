class Price_Brackets:
    low = 0;
    med = 0;
    high = 0;
    other = []
    links = []
    is_available = [];
    image = "";
    def __init__(self, low_, med_, high_, other_=[], links_=[], is_available_ = [], image_=""):
        self.low = low_
        self.med = med_
        self.high = high_
        self.other = other_
        self.links = links_
        self.is_available = is_available_;
        self.image = image_